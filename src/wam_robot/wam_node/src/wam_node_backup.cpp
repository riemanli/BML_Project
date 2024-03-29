/*
 Copyright 2012 Barrett Technology <support@barrett.com>

 This file is part of barrett-ros-pkg.

 This version of barrett-ros-pkg is free software: you can redistribute it
 and/or modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 This version of barrett-ros-pkg is distributed in the hope that it will be
 useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this version of barrett-ros-pkg.  If not, see
 <http://www.gnu.org/licenses/>.

 Barrett Technology holds all copyrights on barrett-ros-pkg. As the sole
 copyright holder, Barrett reserves the right to release future versions
 of barrett-ros-pkg under a different license.

 File: wam_node.cpp
 Date: 5 June, 2012
 Author: Kyle Maroney
 Modifed: Randy Hellman
 Desc: Added joint trajectory execution for ROS moveit along with logging features for both the wam and the BioTacs
       Along with many new services 
 */
#include <iostream>
#include <unistd.h>
#include <math.h>

#include <boost/thread.hpp> // BarrettHand threading
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>

#include <ros/ros.h>
#include <ros/package.h>
#include <tf/transform_datatypes.h>

#include <biotac_sensors/biotac_hand_class.h>
#include <biotac_sensors/BioTacHand.h>

#include <tf/transform_broadcaster.h>

#include <wam_msgs/RTJointPos.h>
#include <wam_msgs/RTJointVel.h>
#include <wam_msgs/RTCartPos.h>
#include <wam_msgs/RTCartVel.h>
#include <wam_msgs/RTOrtnPos.h>
#include <wam_msgs/RTOrtnVel.h>
#include <wam_msgs/GravityComp.h>
#include <wam_msgs/Hold.h>
#include <wam_msgs/JointMove.h>
#include <wam_msgs/PoseMove.h>
#include <wam_msgs/CartPosMove.h>
#include <wam_msgs/OrtnMove.h>
#include <wam_msgs/BHandFingerPos.h>
#include <wam_msgs/BHandGraspPos.h>
#include <wam_msgs/BHandSpreadPos.h>
#include <wam_msgs/BHandFingerVel.h>
#include <wam_msgs/BHandGraspVel.h>
#include <wam_msgs/BHandSpreadVel.h>
#include <wam_msgs/JointStateAndBiotac.h>
//RH included-----------------------------------
#include <wam_msgs/JointTrajectoryDMPRollout.h>
#include <wam_msgs/JointTrajectoryVelocityMove.h>
#include <wam_msgs/LoggerInfo.h>
#include <wam_msgs/SpecialService.h>
//#include "cheetah.h" //BioTac rquired
//#include "BCbiotac.h"
#include <barrett/log.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>

//RH included end-------------------------------
#include <std_srvs/Empty.h>

#include <sensor_msgs/JointState.h>
#include <geometry_msgs/PoseStamped.h>

#include <barrett/math.h> 
#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>
#include <barrett/standard_main_function.h>
#include <barrett/systems/wam.h>
#include <barrett/detail/stl_utils.h>

#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <rosbag/recorder.h>

#include <boost/foreach.hpp>

static const int PUBLISH_FREQ = 100; // Default Control Loop / Publishing Frequency
static const int BHAND_PUBLISH_FREQ = 100; // Publishing Frequency for the BarretHand
static const double SPEED = 0.03; // Default Cartesian Velocity
static const int btBatchDataVectorSize = 25;
//std::vector<sensor_msgs::JointState>
//char tmpFile[] = "/tmp/btXXXXXX";

	//if (mkstemp(tmpFile) == -1) {
	//	ROS_INFO("ERROR: Couldn't create temporary file!\n");
	//	return false;
	//}
using namespace barrett;
using detail::waitForEnter;
using namespace biotac;

//ProductManager PM("/etc/barrett/bus1/default.conf");
//Global parameters for BioTacHandClass
BioTacHandClass LEFT_HAND("left_hand"); //initialize the biotac hand class    



//Creating a templated multiplier for our real-time computation
template<typename T1, typename T2, typename OutputType>
  class Multiplier : public systems::System, public systems::SingleOutput<OutputType>
  {
  public:
    Input<T1> input1;
  public:
    Input<T2> input2;

  public:
    Multiplier(std::string sysName = "Multiplier") :
        systems::System(sysName), systems::SingleOutput<OutputType>(this), input1(this), input2(this)
    {
    }
    virtual ~Multiplier()
    {
      mandatoryCleanUp();
    }

  protected:
    OutputType data;
    virtual void operate()
    {
      data = input1.getValue() * input2.getValue();
      this->outputValue->setData(&data);
    }

  private:
    DISALLOW_COPY_AND_ASSIGN(Multiplier);

  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };

//Creating a templated converter from Roll, Pitch, Yaw to Quaternion for real-time computation
class ToQuaternion : public systems::SingleIO<math::Vector<3>::type, Eigen::Quaterniond>
{
public:
  Eigen::Quaterniond outputQuat;

public:
  ToQuaternion(std::string sysName = "ToQuaternion") :
      systems::SingleIO<math::Vector<3>::type, Eigen::Quaterniond>(sysName)
  {
  }
  virtual ~ToQuaternion()
  {
    mandatoryCleanUp();
  }

protected:
  tf::Quaternion q;
  virtual void operate()
  {
    const math::Vector<3>::type &inputRPY = input.getValue();
    q.setEulerZYX(inputRPY[2], inputRPY[1], inputRPY[0]);
    outputQuat.x() = q.getX();
    outputQuat.y() = q.getY();
    outputQuat.z() = q.getZ();
    outputQuat.w() = q.getW();
    this->outputValue->setData(&outputQuat);
  }

private:
  DISALLOW_COPY_AND_ASSIGN(ToQuaternion);

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

//Simple Function for converting Quaternion to RPY
math::Vector<3>::type toRPY(Eigen::Quaterniond inquat)
{
  math::Vector<3>::type newRPY;
  tf::Quaternion q(inquat.x(), inquat.y(), inquat.z(), inquat.w());
  tf::Matrix3x3(q).getEulerZYX(newRPY[2], newRPY[1], newRPY[0]);
  return newRPY;
}

//WamNode Class
template<size_t DOF>
  class WamNode
  {
    BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
  protected:
    bool cart_vel_status, ortn_vel_status, jnt_vel_status;
    bool jnt_pos_status, cart_pos_status, ortn_pos_status, new_rt_cmd;
    double cart_vel_mag, ortn_vel_mag;
    systems::Wam<DOF>& wam;
    Hand* hand;
    jp_type jp, jp_cmd, jp_home;
    jp_type rt_jp_cmd, rt_jp_rl;
    jv_type rt_jv_cmd;

    cp_type cp_cmd, rt_cv_cmd;
    cp_type rt_cp_cmd, rt_cp_rl;
    Eigen::Quaterniond ortn_cmd, rt_op_cmd, rt_op_rl;
    pose_type pose_cmd;
    math::Vector<3>::type rt_ortn_cmd;
    systems::ExposedOutput<Eigen::Quaterniond> orientationSetPoint, current_ortn;
    systems::ExposedOutput<cp_type> cart_dir, current_cart_pos, cp_track;
    systems::ExposedOutput<math::Vector<3>::type> rpy_cmd, current_rpy_ortn;
    systems::ExposedOutput<jv_type> jv_track;
    systems::ExposedOutput<jp_type> jp_track;
    systems::TupleGrouper<cp_type, Eigen::Quaterniond> rt_pose_cmd;
    systems::Summer<cp_type> cart_pos_sum;
    systems::Summer<math::Vector<3>::type> ortn_cmd_sum;
    systems::Ramp ramp;
    systems::RateLimiter<jp_type> jp_rl;
    systems::RateLimiter<cp_type> cp_rl;
    Multiplier<double, cp_type, cp_type> mult_linear;
    Multiplier<double, math::Vector<3>::type, math::Vector<3>::type> mult_angular;
    ToQuaternion to_quat, to_quat_print;
    Eigen::Quaterniond ortn_print;
    ros::Time last_cart_vel_msg_time, last_ortn_vel_msg_time, last_jnt_vel_msg_time;
    ros::Time last_jnt_pos_msg_time, last_cart_pos_msg_time, last_ortn_pos_msg_time;
    ros::Duration rt_msg_timeout;

    //Subscribed Topics
    wam_msgs::RTCartVel cart_vel_cmd;
    wam_msgs::RTOrtnVel ortn_vel_cmd;

    sensor_msgs::JointState wam_joint_state_msg;
    std::vector<sensor_msgs::JointState> wam_joint_state_msg_vec;

    //Subscribers
    ros::Subscriber cart_vel_sub;
    ros::Subscriber ortn_vel_sub;
    ros::Subscriber jnt_vel_sub;
    ros::Subscriber jnt_pos_sub;
    ros::Subscriber cart_pos_sub;
    ros::Subscriber ortn_pos_sub;
    ros::Subscriber joint_trajectory_velocity_move; //RH

    ros::Subscriber joint_states_sub;

    //Published Topics
    sensor_msgs::JointState wam_joint_state, bhand_joint_state;
    geometry_msgs::PoseStamped wam_pose;
    wam_msgs::JointStateAndBiotac wam_joint_state_and_biotac;


    //Publishers
    ros::Publisher wam_joint_state_pub, bhand_joint_state_pub, wam_pose_pub, wam_joint_state_and_biotac_pub;

    //Services
    ros::ServiceServer gravity_srv, go_home_srv, hold_jpos_srv, hold_cpos_srv;
    ros::ServiceServer hold_ortn_srv, joint_move_srv, pose_move_srv;
    ros::ServiceServer cart_move_srv, ortn_move_srv, hand_close_srv;
    ros::ServiceServer hand_open_grsp_srv, hand_close_grsp_srv, hand_open_sprd_srv;
    ros::ServiceServer hand_close_sprd_srv, hand_fngr_pos_srv, hand_fngr_vel_srv;
    ros::ServiceServer hand_grsp_pos_srv, hand_grsp_vel_srv, hand_sprd_pos_srv;
    ros::ServiceServer hand_sprd_vel_srv;
    ros::ServiceServer joint_vals_recorder_srv;
    ros::ServiceServer joint_trajectory_dmp_rollout_srv;
    //RH
    ros::ServiceServer joint_trajectory_velocity_move_srv;
    //ros::ServiceServer start_record_bioTac_srv;
    ros::ServiceServer special_service_srv;
    ros::ServiceServer demonstration_srv;
    ros::ServiceServer getproductmanager_srv;
    std::string biotacLoggerFilename, biotacLoggerDir, JointValsFilename;

    //tf transform used to publish the transformation between robot base and end effector 
    tf::TransformBroadcaster br_;
    tf::Transform transform_;
    
  public:
    ProductManager *pmPtr;
    wam_msgs::BHandFingerPos currFingerPos;

    WamNode(systems::Wam<DOF>& wam_) :
        wam(wam_), hand(NULL), ramp(NULL, SPEED)
    {
    }
    void
    init(ProductManager& pm);

    ~WamNode()
    {
    }

    //bool
    //startRecordBioTac(wam_msgs::LoggerInfo::Request &req, wam_msgs::LoggerInfo::Response &res);
    //bool
    //specialService(wam_msgs::SpecialService::Request &req, wam_msgs::SpecialService::Response &res);
    bool
    jointTrajVelMove(wam_msgs::JointTrajectoryVelocityMove::Request &req, wam_msgs::JointTrajectoryVelocityMove::Response &res);
    bool 
    jointTrajectoryDMPRollout(wam_msgs::JointTrajectoryDMPRollout::Request &req, wam_msgs::JointTrajectoryDMPRollout::Response &res);
    bool
    gravity(wam_msgs::GravityComp::Request &req, wam_msgs::GravityComp::Response &res);
    bool
    goHome(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    bool
    holdJPos(wam_msgs::Hold::Request &req, wam_msgs::Hold::Response &res);
    bool
    holdCPos(wam_msgs::Hold::Request &req, wam_msgs::Hold::Response &res);
    bool
    holdOrtn(wam_msgs::Hold::Request &req, wam_msgs::Hold::Response &res);
    bool
    jointMove(wam_msgs::JointMove::Request &req, wam_msgs::JointMove::Response &res);
    bool
    poseMove(wam_msgs::PoseMove::Request &req, wam_msgs::PoseMove::Response &res);
    bool
    cartMove(wam_msgs::CartPosMove::Request &req, wam_msgs::CartPosMove::Response &res);
    bool
    ortnMove(wam_msgs::OrtnMove::Request &req, wam_msgs::OrtnMove::Response &res);
    bool
    handOpenGrasp(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    bool
    handCloseGrasp(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    bool
    handOpenSpread(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    bool
    handCloseSpread(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    bool
    handFingerPos(wam_msgs::BHandFingerPos::Request &req, wam_msgs::BHandFingerPos::Response &res);
    bool
    handGraspPos(wam_msgs::BHandGraspPos::Request &req, wam_msgs::BHandGraspPos::Response &res);
    bool
    handSpreadPos(wam_msgs::BHandSpreadPos::Request &req, wam_msgs::BHandSpreadPos::Response &res);
    bool
    handFingerVel(wam_msgs::BHandFingerVel::Request &req, wam_msgs::BHandFingerVel::Response &res);
    bool
    handGraspVel(wam_msgs::BHandGraspVel::Request &req, wam_msgs::BHandGraspVel::Response &res);
    bool
    handSpreadVel(wam_msgs::BHandSpreadVel::Request &req, wam_msgs::BHandSpreadVel::Response &res);
    void
    cartVelCB(const wam_msgs::RTCartVel::ConstPtr& msg);
    void
    ortnVelCB(const wam_msgs::RTOrtnVel::ConstPtr& msg);
    void
    jntVelCB(const wam_msgs::RTJointVel::ConstPtr& msg);
    void
    jntPosCB(const wam_msgs::RTJointPos::ConstPtr& msg);
    void
    cartPosCB(const wam_msgs::RTCartPos::ConstPtr& msg);
    void 
    jointStatesCB(const sensor_msgs::JointState::ConstPtr& msg);
    void
    publishWam();
    void
    publishHand(void);
    //void 
    //bioTac_logging_thread(void); //thread for biotac logging
    //void 
    //display_thread(void); //thread for biotac logging
    void
    updateRT(ProductManager& pm);
    void
    JointPosTorRecording(void);
    bool
    startRecordJointVals(wam_msgs::LoggerInfo::Request &req, wam_msgs::LoggerInfo::Response &res);
    bool 
    Demonstration(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    bool 
    getProductManager(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    template<class MessageType>
    bool writeToBagFile(const std::vector<MessageType>& msgs,
                        const std::string& topic_name,
                        const std::string& abs_bag_file_name,
                        rosbag::BagMode mode = rosbag::bagmode::Write,
                        bool verbose = true);

    template<class MessageType>
    bool writeToBagFile(const MessageType& msg,
                        const std::string& topic_name,
                        const std::string& abs_bag_file_name,
                        rosbag::BagMode mode = rosbag::bagmode::Write,
                        bool verbose = true);

    template<class MessageType>
    bool writeToBagFileWithTimeStamps(const std::vector<MessageType>& msgs,
                                      const std::vector<ros::Time>& times, 
                                      const std::string& topic_name,
                                      const std::string& abs_bag_file_name,
                                      rosbag::BagMode mode = rosbag::bagmode::Write,
                                      bool verbose = true);

    template<class MessageType>
    bool readFromBagFile(MessageType& msg,
                         const std::string& topic_name,
                         const std::string& abs_bag_file_name,
                         bool verbose = true);

    template<class MessageType>
    bool readFromBagFile(std::vector<MessageType>& msgs,
                         const std::string& topic_name,
                         const std::string& abs_bag_file_name,
                         bool verbose = true);



    


  };

// Bad file reading function 
    template<size_t DOF>
    template<class MessageType>
    bool WamNode<DOF>::readFromBagFile(MessageType& msg,
                                       const std::string& topic_name,
                                       const std::string& abs_bag_file_name,
                                       bool verbose)
    {
        std::vector<MessageType> msgs;
        if (!readFromBagFile(msgs, topic_name, abs_bag_file_name, verbose))
        {
            ROS_ERROR("Could not read messages from topic >%s< from bag file >%s<.", topic_name.c_str(), abs_bag_file_name.c_str());
            return false;
        }

        if (msgs.size() != 1)
        {
            ROS_ERROR("There are >%i< messages contained under the topic named >%s< in the bag file >%s<, cannot read single message.", (int)msgs.size(), topic_name.c_str(), abs_bag_file_name.c_str());
            return false;
        }

        msg = msgs[0];
        return true;
    }

    template<size_t DOF>
    template<class MessageType>
    bool WamNode<DOF>::readFromBagFile(std::vector<MessageType>& msgs,
                                       const std::string& topic_name,
                                       const std::string& abs_bag_file_name,
                                       bool verbose)
    {
        ROS_INFO_COND(verbose, "Reading topic named >%s< from bag file >%s<", topic_name.c_str(), abs_bag_file_name.c_str());
        try
        {
            rosbag::Bag bag(abs_bag_file_name, rosbag::bagmode::Read);
            rosbag::View view(bag, rosbag::TopicQuery(topic_name));

            msgs.clear();
            BOOST_FOREACH(rosbag::MessageInstance const msg_instance, view)
            {
                typename MessageType::ConstPtr msg = msg_instance.instantiate<MessageType>();
                if (msg != NULL)
                {
                    msgs.push_back(*msg);
                }
                else
                {
                    ROS_ERROR("NUll message read from file >%s< on topic >%s<.", abs_bag_file_name.c_str(), topic_name.c_str());
                    return false;
                }
            }
            bag.close();
        }
        catch (rosbag::BagIOException ex)
        {
            ROS_ERROR("Problem when reading from bag file >%s< : %s.", abs_bag_file_name.c_str(), ex.what());
            return false;
        }
        ROS_INFO_COND(verbose, "Read >%i< messages.", (int)msgs.size());
        ROS_WARN_COND(msgs.empty(), "No messages read from file >%s< on topic >%s<.", abs_bag_file_name.c_str(), topic_name.c_str());
        return true;
    }

// Bag file writing function 
    template<size_t DOF>
    template<class MessageType>
    bool WamNode<DOF>::writeToBagFile(const std::vector<MessageType>& msgs,
                                      const std::string& topic_name,
                                      const std::string& abs_bag_file_name,
                                      rosbag::BagMode mode,
                                      bool verbose)
    {
        std::vector<ros::Time> times;
        for (int i = 0; i< (int)msgs.size(); i++)
        {
            times.push_back(msgs[i].header.stamp);
        }
        return writeToBagFileWithTimeStamps(msgs, times, topic_name, abs_bag_file_name, mode, verbose);
    }


    template<size_t DOF>
    template<class MessageType>
    bool WamNode<DOF>::writeToBagFile(const MessageType& msg,
                        const std::string& topic_name,
                        const std::string& abs_bag_file_name,
                        rosbag::BagMode mode,
                        bool verbose)
    {
        std::vector<MessageType> msgs;
        msgs.push_back(msg);
        ROS_INFO("Writing message into bag file >%s<", abs_bag_file_name.c_str());
        return writeToBagFile(msgs, topic_name, abs_bag_file_name, mode, verbose);
    }


    template<size_t DOF>
    template<class MessageType>
    bool WamNode<DOF>::writeToBagFileWithTimeStamps(const std::vector<MessageType>& msgs,
                                                    const std::vector<ros::Time>& times,
                                                    const std::string& topic_name,
                                                    const std::string& abs_bag_file_name,
                                                    rosbag::BagMode mode,
                                                    bool verbose)
    {
        ROS_ASSERT_MSG(!msgs.empty(), "Messages are empty. Cannot write anything to file >%s<.", abs_bag_file_name.c_str());
        ROS_INFO_COND(verbose, "Writing to bag file: %s", abs_bag_file_name.c_str());
        ROS_INFO("%i messages in %i timestamps", int(msgs.size()), int(times.size()));
        try
        {
            rosbag::Bag bag(abs_bag_file_name, mode);
            ROS_ASSERT(msgs.size() == times.size());
            for (int i = 0; i < (int)msgs.size(); ++i)
            {
                bag.write(topic_name, times[i], msgs[i]);
            }
            bag.close();
        }

        catch (rosbag::BagIOException ex)
        {
            ROS_ERROR("Problem when wrtting to bag file named >%s< : %s.", abs_bag_file_name.c_str(), ex.what());
            return false;
        }

        return true;
    }
// Templated Initialization Function
template<size_t DOF>
  void WamNode<DOF>::init(ProductManager& pm)
  {
   
    pmPtr = &pm;

    ros::NodeHandle n_(""); // WAM specific nodehandle
    //ros::NodeHandle n_2("wam_r_jointvals");
    ros::NodeHandle nh_("bhand_r"); // BarrettHand specific nodehandle

    //initialize the biotac sensor 
    //LEFT_HAND.initBioTacSensors();

    //Setting up real-time command timeouts and initial values
    cart_vel_status = false; //Bool for determining cartesian velocity real-time state
    ortn_vel_status = false; //Bool for determining orientation velocity real-time state
    jnt_vel_status = false; // Bool for dertermining joint velocity real-time state
    jnt_pos_status = true; //Bool for determining joint position real-time state
    cart_pos_status = false; //Bool for determining cartesian position real-time state
    new_rt_cmd = false; //Bool for determining if a new real-time message was received
    rt_msg_timeout.fromSec(0.03); //rt_status will be determined false if rt message is not received in specified time
    cart_vel_mag = SPEED; //Setting default cartesian velocity magnitude to SPEED
    ortn_vel_mag = SPEED;
    pm.getExecutionManager()->startManaging(ramp); //starting ramp manager
    ROS_INFO_STREAM("Joint Position Status: " << jnt_pos_status << " !");

    ROS_INFO(" \n %zu-DOF WAM right", DOF);
    jp_home = wam.getJointPositions();

    if (pm.foundHand()) //Does the following only if a BarrettHand is present
    {
      std::cout << "Barrett Hand right" << std::endl;
      hand = pm.getHand();

      // Adjust the torque limits to allow for BarrettHand movements at extents
      pm.getSafetyModule()->setTorqueLimit(3.0);

      // Move j3 in order to give room for hand initialization
      jp_type jp_init = wam.getJointPositions();
      jp_init[3] -= 0.35;
      usleep(500000);
      wam.moveTo(jp_init);

      usleep(500000);
      hand->initialize();
      hand->update();

      //Publishing the following topics only if there is a BarrettHand present
      bhand_joint_state_pub = nh_.advertise < sensor_msgs::JointState > ("joint_states_r", 1); // bhand/joint_states

      //Advertise the following services only if there is a BarrettHand present
      hand_open_grsp_srv  = nh_.advertiseService("open_grasp", &WamNode<DOF>::handOpenGrasp, this); // bhand/open_grasp
      hand_close_grsp_srv = nh_.advertiseService("close_grasp", &WamNode::handCloseGrasp, this); // bhand/close_grasp
      hand_open_sprd_srv  = nh_.advertiseService("open_spread", &WamNode::handOpenSpread, this); // bhand/open_spread
      hand_close_sprd_srv = nh_.advertiseService("close_spread", &WamNode::handCloseSpread, this); // bhand/close_spread
      hand_fngr_pos_srv   = nh_.advertiseService("finger_pos", &WamNode::handFingerPos, this); // bhand/finger_pos
      hand_grsp_pos_srv   = nh_.advertiseService("grasp_pos", &WamNode::handGraspPos, this); // bhand/grasp_pos
      hand_sprd_pos_srv   = nh_.advertiseService("spread_pos", &WamNode::handSpreadPos, this); // bhand/spread_pos
      hand_fngr_vel_srv   = nh_.advertiseService("finger_vel", &WamNode::handFingerVel, this); // bhand/finger_vel
      hand_grsp_vel_srv   = nh_.advertiseService("grasp_vel", &WamNode::handGraspVel, this); // bhand/grasp_vel
      hand_sprd_vel_srv   = nh_.advertiseService("spread_vel", &WamNode::handSpreadVel, this); // bhand/spread_vel

      //Set up the BarrettHand joint state publisher
      const char* bhand_jnts[] = {"inner_f1_r", "inner_f2_r", "inner_f3_r", "spread_r", "outer_f1_r", "outer_f2_r", "outer_f3_r"};
      std::vector < std::string > bhand_joints(bhand_jnts, bhand_jnts + 7);
      bhand_joint_state.name.resize(7);
      bhand_joint_state.name = bhand_joints;
      bhand_joint_state.position.resize(7);
    }

    wam.gravityCompensate(true); // Turning on Gravity Compenstation by Default when starting the WAM Node

    //Setting up WAM joint state publisher
    const char* wam_jnts[] = {"left_wam/base_yaw_joint", "left_wam/shoulder_pitch_joint", "left_wam/shoulder_yaw_joint", "left_wam/elbow_pitch_joint", "left_wam/wrist_yaw_joint", "left_wam/wrist_pitch_joint", "left_wam/palm_yaw_joint"};
    std::vector < std::string > wam_joints(wam_jnts, wam_jnts + 7);
    wam_joint_state.name = wam_joints;
    wam_joint_state.name.resize(DOF);
    wam_joint_state.position.resize(DOF);
    wam_joint_state.velocity.resize(DOF);
    wam_joint_state.effort.resize(DOF);

    //Publishing the following rostopics
    wam_joint_state_pub = n_.advertise < sensor_msgs::JointState > ("joint_states", 1); // wam/joint_states
    wam_joint_state_and_biotac_pub = n_.advertise< wam_msgs::JointStateAndBiotac >("joint_states_and_biotac",10); // wam/joint_states_and_biotac
    wam_pose_pub = n_.advertise < geometry_msgs::PoseStamped > ("pose_r", 1); // wam/pose

    //Subscribing to the following rostopics
    cart_vel_sub   = n_.subscribe("cart_vel_cmd", 1, &WamNode::cartVelCB, this); // wam/cart_vel_cmd
    ortn_vel_sub   = n_.subscribe("ortn_vel_cmd", 1, &WamNode::ortnVelCB, this); // wam/ortn_vel_cmd
    jnt_vel_sub    = n_.subscribe("jnt_vel_cmd", 1, &WamNode::jntVelCB, this); // wam/jnt_vel_cmd
    jnt_pos_sub    = n_.subscribe("jnt_pos_cmd", 1, &WamNode::jntPosCB, this); // wam/jnt_pos_cmd
    cart_pos_sub   = n_.subscribe("cart_pos_cmd", 1, &WamNode::cartPosCB, this); // wam/cart_pos_cmd

    joint_states_sub = n_.subscribe("joint_states", 1000, &WamNode::jointStatesCB, this);


    //Advertising the following rosservices
    gravity_srv    = n_.advertiseService("gravity_comp", &WamNode::gravity, this); // wam/gravity_comp
    go_home_srv    = n_.advertiseService("go_home", &WamNode::goHome, this); // wam/go_home
    hold_jpos_srv  = n_.advertiseService("hold_joint_pos", &WamNode::holdJPos, this); // wam/hold_joint_pos
    hold_cpos_srv  = n_.advertiseService("hold_cart_pos", &WamNode::holdCPos, this); // wam/hold_cart_pos
    hold_ortn_srv  = n_.advertiseService("hold_ortn", &WamNode::holdOrtn, this); // wam/hold_ortn
    joint_move_srv = n_.advertiseService("joint_move", &WamNode::jointMove, this); // wam/joint_move
    pose_move_srv  = n_.advertiseService("pose_move", &WamNode::poseMove, this); // wam/pose_move
    cart_move_srv  = n_.advertiseService("cart_move", &WamNode::cartMove, this); // wam/cart_pos_move
    ortn_move_srv  = n_.advertiseService("ortn_move", &WamNode::ortnMove, this); // wam/ortn_move
    //RH wam/joint_trajectory_velocity_move
    joint_trajectory_dmp_rollout_srv = n_.advertiseService("joint_trajectory_dmp_rollout", &WamNode::jointTrajectoryDMPRollout, this);
    joint_trajectory_velocity_move_srv = n_.advertiseService("joint_trajectory_velocity_move", &WamNode::jointTrajVelMove, this);
    //start_record_bioTac_srv            = n_.advertiseService("logger_controller", &WamNode::startRecordBioTac, this);
    //special_service_srv                = n_.advertiseService("special_service", &WamNode::specialService, this);
    //joint_vals_recorder_srv            = n_2.advertiseService("joint_vals_recorder", &WamNode::startRecordJointVals, this);
    demonstration_srv = n_.advertiseService("Demonstration", &WamNode::Demonstration, this);
    getproductmanager_srv = n_.advertiseService("getProductManager", &WamNode::getProductManager, this);
    biotacLoggerDir = "/home/robot/biotacRecordFiles/";
    biotacLoggerFilename = "/home/robot/ros-book/BioTacVal.csv";
    JointValsFilename = "/home/robot/ros-book/JointVals.csv";
 
  
    std::string path = ros::package::getPath("wam_node");

    ROS_INFO("Number of subscribers for /joint_states topic: %i", wam_joint_state_pub.getNumSubscribers());
    ROS_INFO("String pkgDir - %s", path.c_str());


  }




bool cheetahOn = false;
bool shouldLogBioTacs = true;
bool shouldLogJointVals = false;

template<size_t DOF>
  bool WamNode<DOF>::getProductManager(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
  {
    return true;
  }
/*
template<size_t DOF>
  void WamNode<DOF>::display_thread(void)
  {libarrett limit rate
    btsleep(0.5);
    btBase = data;
    while(cheetahOn)
    {   
      ROS_INFO("bt.pdc - [ %d, %d]", data.bt[0].pdc - btBase.bt[0].pdc, data.bt[0].pdc);  
      btsleep(0.5);
    }libarrett limit rate
  }
*/

template<size_t DOF>
  void WamNode<DOF>::JointPosTorRecording(void)
  {
    //char tmpFile[] = "/tmp/rt_msg_timeoutbtXXXXXX";
	//if (mkstemp(tmpFile) == -1) {
	//	ROS_INFO("ERROR: Couldn't create temporary file!\n");
     	//return false;
	//}
    shouldLogJointVals = true;
    systems::Ramp time(pmPtr->getExecutionManager());

	systems::TupleGrouper<double, jp_type, jv_type, jt_type> tg;//, cp_type, Eigen::Quaterniond> tg;
    //typedef boost::tuple<double, math::Vector<BT_NUM_VAR_RECORD>::type, jp_type> tuple_type;
	connect(time.output, tg.template getInput<0>());
	connect(wam.jpOutput, tg.template getInput<1>());
	connect(wam.jvOutput, tg.template getInput<2>());
	connect(wam.jtSum.output, tg.template getInput<3>());
	//connect(wam.toolPosition.output, tg.template getInput<4>());
	//connect(wam.toolOrientation.output, tg.template getInput<5>());

    const size_t PERIOD_MULTIPLIER = 5;
    typedef boost::tuple<double, jp_type, jv_type, jt_type> tuple_type;//, cp_type, Eigen::Quaterniond> tuple_type;

    char tmpFile[] = "/tmp/btXXXXXX";
    if (mkstemp(tmpFile) == -1) {
        printf("ERROR: Couldn't create temporary file!\n");
       
    }    
	systems::PeriodicDataLogger<tuple_type> logger(
	    pmPtr->getExecutionManager(),
	    new log::RealTimeWriter<tuple_type>(tmpFile, PERIOD_MULTIPLIER * pmPtr->getExecutionManager()->getPeriod()),
	    PERIOD_MULTIPLIER);
    //typedef boost::tuple<double, jp_type, jv_type, jt_type, cp_type, Eigen::Quaterniond> tuple_type;
	//time.start();
    //if(req.record == 1) //start JointPosTor record
    //{ 
       //typedef boost::tuple<double, jp_type, jv_type, jt_type, cp_type, Eigen::Quaterniond> tuple_type;
	   

	time.start();
	connect(tg.output, logger.input);
	ROS_INFO("Logging started.\n");
      
    //}
    //else if(req.record == 2) //stop JointPosTor record and save an .csv file
    while (shouldLogJointVals)
    {
       usleep(0.1);
    }
    logger.closeLog();
    ROS_INFO ("Logging stopped.\n");

	log::Reader<tuple_type> lr(tmpFile);
    lr.exportCSV(JointValsFilename.c_str());
    ROS_INFO("Output written to %s.\n",JointValsFilename.c_str());
    std::remove(tmpFile);
    
    //return true;
    //}
  }

boost::thread_group JointValsLoggingThread;
template<size_t DOF>
  bool WamNode<DOF>::startRecordJointVals(wam_msgs::LoggerInfo::Request &req, wam_msgs::LoggerInfo::Response &res)
  {
   if(req.record == 1)
   {
      JointValsLoggingThread.create_thread( boost::bind(&WamNode<DOF>::JointPosTorRecording, this) );
      ROS_INFO("Started JointValsLoggingThread");
      res.success = 1;
   }

   else if(req.record == 2)
   {
      shouldLogJointVals = false;
      JointValsLoggingThread.join_all();
      ROS_INFO("JointValsLoggingThread.join_all()");
      res.success = 0;
   }
  
   ROS_INFO("startRecordJointVals hit");

   return true;
  }
/*
template<size_t DOF>
  bool WamNode<DOF>::specialService(wam_msgs::SpecialService::Request &req, wam_msgs::SpecialService::Response &res)
  {
    //wam_msgs::BHandFingerPos currFingerPos;
    double pdcDeltaSet = req.value1;
    double increment   = req.value2;
    if(req.action.compare("setPdc") ==0)//cheetahOn)
    {
      if(pdcDeltaSet < 50)
      {
        pdcDeltaSet = 50;
      }
      if(increment == 0 || increment > 0.2)
      {
        increment = 0.01;
      }

      while( (data.bt[0].pdc - btBase.bt[0].pdc) < req.value1)
      {
        ROS_INFO("B -F1: %4.2f, F2: %4.2f",currFingerPos.request.radians[0], currFingerPos.request.radians[1]);
        currFingerPos.request.radians[0] = currFingerPos.request.radians[0] + increment;
        currFingerPos.request.radians[1] = currFingerPos.request.radians[1] + increment;
        hand->trapezoidalMove(Hand::jp_type(currFingerPos.request.radians[0], currFingerPos.request.radians[1], currFingerPos.request.radians[2], 0.0), Hand::GRASP, false);
        btsleep(0.25);
      }
      res.status = "completed setPdc";
    }
    else if(req.action.compare("resetPinch") == 0)
    {
      double pinchStart = 1.88;
      hand->trapezoidalMove(Hand::jp_type( 0, 0, currFingerPos.request.radians[2], 0.0), Hand::GRASP, false);
      btsleep(1.3);
      hand->trapezoidalMove(Hand::jp_type( pinchStart, pinchStart, currFingerPos.request.radians[2], 0.0), Hand::GRASP, false);
      btsleep(0.75);
      currFingerPos.request.radians[0] = pinchStart;
      currFingerPos.request.radians[1] = pinchStart;
      
      res.status = "completed resetPinch";
    }
    else if(req.action.compare("getBioTacBatch") == 0)
    {
      printf("\n");
      
      for(int i = 0; i<19 ; i++)
      {
        res.bt.push_back(0);
        res.bt[i] = 0;
        for(long unsigned j=0; j < bioTacBatchData.size(); j++)
        {
          res.bt[i] += bioTacBatchData[j].bt[0].elec[i];
        }
        res.bt[i] = int(double(res.bt[i])/double(btBatchDataVectorSize));
      }

      for(int i = 0; i<19 ; i++)
      {
        res.bt.push_back(0);
        res.bt[i+19] = 0;
        for(long unsigned j=0; j < bioTacBatchData.size(); j++)
        {
          res.bt[i+19] += bioTacBatchData[j].bt[1].elec[i];
        }
        res.bt[i+19] = int(double(res.bt[i+19])/double(btBatchDataVectorSize));
      }

      res.success = bioTacBatchData.size();
    }
    else
    {
      res.status = "specialService FAILED";
    }

    return true;
  }
*/

template<size_t DOF>
  bool WamNode<DOF>::jointTrajectoryDMPRollout(wam_msgs::JointTrajectoryDMPRollout::Request &req, wam_msgs::JointTrajectoryDMPRollout::Response &res)
  {
      ROS_INFO("jointTrajectoryDMPRolloutMove CALLBACK ************");
      trajectory_msgs::JointTrajectory JointTrajectory;
      std::string topic_name = "/trajectory_msgs";
      std::string package_path = ros::package::getPath("wam_node");
      std::string abs_bag_file_name = package_path + "/" + req.trajectory_bag_file_name;
      //std::cout << abs_bag_file_name << std::endl;
      readFromBagFile(JointTrajectory, topic_name, abs_bag_file_name);
      ROS_INFO("The trajectory has >%i< waypoints.", int(JointTrajectory.points.size()));

      double sampling_frequency = (JointTrajectory.points[JointTrajectory.points.size() - 1].time_from_start.toSec() - JointTrajectory.points[0].time_from_start.toSec());
      ROS_INFO("sampling frequency: %f", double(JointTrajectory.points.size() / sampling_frequency));

      double timePerWayPoint = 1.0 / JointTrajectory.points.size() * sampling_frequency;
      ROS_INFO("time stamps per way point is : %f", timePerWayPoint);
      

      systems::Ramp time(pmPtr->getExecutionManager());
      typedef boost::tuple<double, jp_type> jp_sample_type;
      jp_sample_type currSample;
      std::vector<jp_sample_type> vec;
      jp_type jp_cmd_curr;

      double currTimeStamp(0);
      int numOfWayPoints = JointTrajectory.points.size() / 2;
      for (int i = 0; i < numOfWayPoints; i++)
      {
          for (size_t j = 0; j < DOF; j++)
          {
              jp_cmd_curr[j] = JointTrajectory.points[i].positions[j];
          }
          currSample = boost::make_tuple(currTimeStamp, jp_cmd_curr);
          vec.push_back(currSample);
          currTimeStamp += timePerWayPoint;
      }
      math::Spline<jp_type> spline(vec);

      // first move to the start position 
      ROS_INFO("MOVING TO THE START OF TRAJ");
      wam.moveTo(spline.eval(spline.initialS()));
    
      // Then play back the recorded motioi
      ROS_INFO("PLAY BACK THE RECORDED MOTION");
      time.stop();
      time.setOutput(spline.initialS());

      systems::Callback<double, jp_type> trajectory(boost::ref(spline));
      connect(time.output, trajectory.input);
      wam.trackReferenceSignal(trajectory.output);
    
      ROS_INFO("TIME START");
      time.start();

      while (trajectory.input.getValue() < spline.finalS()) {
       usleep(100000);
      }

      ROS_INFO("START MOVING");
      wam.moveTo(wam.getJointPositions());
      //wam.gravityCompensate(req.gravity);
      //ROS_INFO("Gravity Compensation Request: %s", (req.gravity) ? "true" : "false");
      res.status = 1;
      
      btsleep(0.1);
      wam.moveHome();
      return res.status;
      
  }
template<size_t DOF>
  bool WamNode<DOF>::jointTrajVelMove(wam_msgs::JointTrajectoryVelocityMove::Request &req, wam_msgs::JointTrajectoryVelocityMove::Response &res)
  {
    ROS_INFO("JointTrajectoryVelocityMove CALLBACK ****************");
    typedef boost::tuple<double, jp_type> jp_sample_type;

    systems::Ramp time(pmPtr->getExecutionManager());
    
    jp_sample_type currSample;
    std::vector<jp_sample_type> vec;
    jp_type jp_cmd_curr;
    
    int numOfWayPoints = (int)req.jointTrajectory.points.size();
    
    //Build timestamps for travelLength and velocity desired
    double timePerWayPoint = (req.length/req.velocity)/ numOfWayPoints;
    double currTimeStamp(0);

    //Build jp_sample_type waypoints
    for( int i=0; i < numOfWayPoints; i++)
    {
      for (size_t j= 0; j < DOF; j++)
      {
        jp_cmd_curr[j] = req.jointTrajectory.points[i].positions[j];
      }
      currSample = boost::make_tuple( currTimeStamp, jp_cmd_curr);
      vec.push_back( currSample );
      //Increase time for next waypoint to be used in spline
      currTimeStamp += timePerWayPoint;
    }

    math::Spline<jp_type> spline(vec);
  
    // first move to the start position 
    ROS_INFO("MOVING TO THE START OF TRAJ");
    wam.moveTo(spline.eval(spline.initialS()));
    
    // Then play back the recorded motion
    time.stop();
    time.setOutput(spline.initialS());

    systems::Callback<double, jp_type> trajectory(boost::ref(spline));
    connect(time.output, trajectory.input);
    wam.trackReferenceSignal(trajectory.output);

    time.start();

    while (trajectory.input.getValue() < spline.finalS()) {
      usleep(100000);
    }    
    wam.moveTo(wam.getJointPositions());
    //wam.gravityCompensate(req.gravity);
    //ROS_INFO("Gravity Compensation Request: %s", (req.gravity) ? "true" : "false");
    res.status = 1;
    return true;
  }

// gravity_comp service callback
template<size_t DOF>
  bool WamNode<DOF>::gravity(wam_msgs::GravityComp::Request &req, wam_msgs::GravityComp::Response &res)
  {
    wam.gravityCompensate(req.gravity);
    ROS_INFO("Gravity Compensation Request: %s", (req.gravity) ? "true" : "false");
    return true;
  }

// goHome Function for sending the WAM safely back to its home starting position.
template<size_t DOF>
  bool WamNode<DOF>::goHome(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
  {
    ROS_INFO("Returning to Home Position");

    if (hand != NULL)
    {
      hand->open(Hand::GRASP, true);
      hand->close(Hand::SPREAD, true);
    }

    ROS_INFO("Moving to home position: ");
    wam.moveHome();
    return true;
  }

//Function to hold WAM Joint Positions
template<size_t DOF>
  bool WamNode<DOF>::holdJPos(wam_msgs::Hold::Request &req, wam_msgs::Hold::Response &res)
  {
    ROS_INFO("Joint Position Hold request: %s", (req.hold) ? "true" : "false");

    if (req.hold)
      wam.moveTo(wam.getJointPositions());
    else
      wam.idle();
    return true;
  }

//Function to hold WAM end effector Cartesian Position
template<size_t DOF>
  bool WamNode<DOF>::holdCPos(wam_msgs::Hold::Request &req, wam_msgs::Hold::Response &res)
  {
    ROS_INFO("Cartesian Position Hold request: %s", (req.hold) ? "true" : "false");

    if (req.hold)
      wam.moveTo(wam.getToolPosition());
    else
      wam.idle();
    return true;
  }

//Function to hold WAM end effector Orientation
template<size_t DOF>
  bool WamNode<DOF>::holdOrtn(wam_msgs::Hold::Request &req, wam_msgs::Hold::Response &res)
  {
    ROS_INFO("Orientation Hold request: %s", (req.hold) ? "true" : "false");

    if (req.hold)
    {
      orientationSetPoint.setValue(wam.getToolOrientation());
      wam.trackReferenceSignal(orientationSetPoint.output);
    }
    else
      wam.idle();
    return true;
  }

//Function to command a joint space move to the WAM
template<size_t DOF>
  bool WamNode<DOF>::jointMove(wam_msgs::JointMove::Request &req, wam_msgs::JointMove::Response &res)
  {
    if (req.joints.size() != DOF)
    {
      ROS_INFO("Request Failed: %zu-DOF request received, must be %zu-DOF", req.joints.size(), DOF);
      return false;
    }
    ROS_INFO("Moving Robot to Commanded Joint Pose");
    for (size_t i = 0; i < DOF; i++)
      jp_cmd[i] = req.joints[i];
    wam.moveTo(jp_cmd, true); //Should wait until complete to return
    return true;
  }

//Function to command a pose move to the WAM
template<size_t DOF>
  bool WamNode<DOF>::poseMove(wam_msgs::PoseMove::Request &req, wam_msgs::PoseMove::Response &res)
  {
    ROS_INFO("Moving Robot to Commanded Pose");

    cp_cmd[0] = req.pose.position.x;
    cp_cmd[1] = req.pose.position.y;
    cp_cmd[2] = req.pose.position.z;
    ortn_cmd.x() = req.pose.orientation.x;
    ortn_cmd.y() = req.pose.orientation.y;
    ortn_cmd.z() = req.pose.orientation.z;
    ortn_cmd.w() = req.pose.orientation.w;

    pose_cmd = boost::make_tuple(cp_cmd, ortn_cmd);

    //wam.moveTo(pose_cmd, false); //(TODO:KM Update Libbarrett API for Pose Moves)
    ROS_INFO("Pose Commands for WAM not yet supported by API");
    return false;
  }

//Function to command a cartesian move to the WAM
template<size_t DOF>
  bool WamNode<DOF>::cartMove(wam_msgs::CartPosMove::Request &req, wam_msgs::CartPosMove::Response &res)
  {
    ROS_INFO("Moving Robot to Commanded Cartesian Position");

    for (int i = 0; i < 3; i++)
      cp_cmd[i] = req.position[i];
    wam.moveTo(cp_cmd, false);
    return true;
  }

//Function to command an orientation move to the WAM
template<size_t DOF>
  bool WamNode<DOF>::ortnMove(wam_msgs::OrtnMove::Request &req, wam_msgs::OrtnMove::Response &res)
  {
    ROS_INFO("Moving Robot to Commanded End Effector Orientation");

    ortn_cmd.x() = req.orientation[0];
    ortn_cmd.y() = req.orientation[1];
    ortn_cmd.z() = req.orientation[2];
    ortn_cmd.w() = req.orientation[3];

    wam.moveTo(ortn_cmd, false);
    return true;
  }

//Function to open the BarrettHand Grasp
template<size_t DOF>
  bool WamNode<DOF>::handOpenGrasp(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
  {
    ROS_INFO("Opening the BarrettHand Grasp");
    hand->open(Hand::GRASP, false);
    return true;
  }

//Function to close the BarrettHand Grasp
template<size_t DOF>
  bool WamNode<DOF>::handCloseGrasp(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
  {
    ROS_INFO("Closing the BarrettHand Grasp");
    hand->close(Hand::GRASP, false);
    return true;
  }

//Function to open the BarrettHand Spread
template<size_t DOF>
  bool WamNode<DOF>::handOpenSpread(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
  {
    ROS_INFO("Opening the BarrettHand Spread");
    hand->open(Hand::SPREAD, true);
    return true;
  }

//Function to close the BarrettHand Spread
template<size_t DOF>
  bool WamNode<DOF>::handCloseSpread(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
  {
    ROS_INFO("Closing the BarrettHand Spread");
    hand->close(Hand::SPREAD, true);
    return true;
  }

template<size_t DOF>
  bool WamNode<DOF>::Demonstration(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
{
    // contrcut the rosbag recorder 
    std::string topic_name = "/joint_states";
    std::string abs_bag_file_name = "/home/robot/kenny_ws/src/joint_states_test.bag";
    rosbag::RecorderOptions options;
    options.prefix = abs_bag_file_name;
    std::vector<std::string> topics;
    topics.push_back(topic_name);
    options.topics = topics;
    rosbag::Recorder record(options);

    
    wam.gravityCompensate();

    systems::Ramp time(pmPtr->getExecutionManager());
    
    hand->open(Hand::GRASP, false);
    printf("Start to move the right wam to start position in 2 secs.\n");
    waitForEnter();
    //record.run();
    
    // subscribe to the joint states topic
    //ros::NodeHandle n_temp("");
    //joint_states_sub = n_temp.subscribe("joint_states", 1000, jointStatesCB_copy);
    //ROS_INFO("The number of subscribers for /joint_states topic is: %i", wam_joint_state_pub.getNumSubscribers());

    wam.idle();
    {
        BARRETT_SCOPED_LOCK(pmPtr->getExecutionManager()->getMutex());
    }

    printf("Press [Enter] to finish moving the wam to start position.\n");
    waitForEnter();


    //joint_states_sub.shutdown();
    //ROS_INFO("The number of subscribers for /joint_states topic is: %i", wam_joint_state_pub.getNumSubscribers());
    //std::string topic_name = "/joint_states";
    //std::string abs_bag_file_name = "/home/robot/kenny_ws/src/joint_states_test.bag";
    //ROS_ASSERT(writeToBagFile(wam_joint_state_msg_vec, topic_name, abs_bag_file_name));
    //wam_joint_state_msg_vec.clear();
    //ROS_INFO("The size of joint state msg vector is: %i", int(wam_joint_state_msg_vec.size()));

    wam.gravityCompensate();
    //wam.moveTo(wam.getJointPositions());
  
    //hand->trapezoidalMove(Hand::jp_type(1.5708), Hand::SPREAD, false);
    //hand->waitUntilDoneMoving();
    //hand->trapezoidalMove(Hand::jp_type(1.7, 1.7, 1.5, 0.0), Hand::GRASP, false);
    //double current_finger_pos = 1.7;

    //printf("Press [Enter] to grasp the peeler.\n");
    //waitForEnter();
    //while (current_finger_pos <= 1.83)
    //{
    //  current_finger_pos = current_finger_pos + 0.01;
    //  hand->trapezoidalMove(Hand::jp_type(current_finger_pos, current_finger_pos, 1.5, 0.0), Hand::GRASP, false);
    //  btsleep(2.0);
    //}
  
    //current_finger_pos = 1.5;
    //while (current_finger_pos <= 1.58)
    //{
    //  current_finger_pos = current_finger_pos + 0.01;
    //  hand->trapezoidalMove(Hand::jp_type(1.83, 1.83, current_finger_pos, 0.0), Hand::GRASP, false);
    //  btsleep(2.0);
    //}

    //printf("Press [Enter] to move the right wam to start demo.\n");
    //waitForEnter();
    //wam.idle();
    //{
    //    BARRETT_SCOPED_LOCK(pmPtr->getExecutionManager()->getMutex());
    //}

    //printf("Press [Enter] to finish moving the wam to stop demo.\n");
    //waitForEnter();

    //wam.gravityCompensate();    

    //printf("Press [Enter] to idle the wam.\n");
    //waitForEnter();
    //wam.idle();
    //ros::shutdown();
    //pmPtr->getSafetyModule()->waitForMode(SafetyModule::IDLE);

    //printf("Press [Enter] to move the right wam to go home.\n");
    //waitForEnter();
    //ROS_INFO("Moving to home position: ");
    //wam.moveHome();

    return true;
}

template<size_t DOF>
void WamNode<DOF>::jointStatesCB(const sensor_msgs::JointState::ConstPtr& msg)
{
    wam_joint_state_msg = *msg;
    //ROS_INFO("The number of subscribers for /joint_states topic is: %i", wam_joint_state_pub.getNumSubscribers());
    //wam_joint_state_msg_vec.push_back(wam_joint_state_msg);

    //ROS_INFO("New joint state msgs received !");
    //std::string topic_name = "/joint_states";
    //std::string abs_bag_file_name = "/home/robot/kenny_ws/src/joint_states_test.bag";
    //ROS_ASSERT(writeToBagFile(wam_joint_state_msg, topic_name, abs_bag_file_name));
}





//Function to control a BarrettHand Finger Position
template<size_t DOF>
  bool WamNode<DOF>::handFingerPos(wam_msgs::BHandFingerPos::Request &req, wam_msgs::BHandFingerPos::Response &res)
  {
    currFingerPos.request = req;
    ROS_INFO("Moving BarrettHand to Finger Positions: %.3f, %.3f, %.3f radians", req.radians[0], req.radians[1],
             req.radians[2]);
    hand->trapezoidalMove(Hand::jp_type(req.radians[0], req.radians[1], req.radians[2], 0.0), Hand::GRASP, false);
    return true;
  }

//Function to control the BarrettHand Grasp Position
template<size_t DOF>
  bool WamNode<DOF>::handGraspPos(wam_msgs::BHandGraspPos::Request &req, wam_msgs::BHandGraspPos::Response &res)
  {
    ROS_INFO("Moving BarrettHand Grasp: %.3f radians", req.radians);
    hand->trapezoidalMove(Hand::jp_type(req.radians), Hand::GRASP, false);
    return true;
  }

//Function to control the BarrettHand Spread Position
template<size_t DOF>
  bool WamNode<DOF>::handSpreadPos(wam_msgs::BHandSpreadPos::Request &req, wam_msgs::BHandSpreadPos::Response &res)
  {
    ROS_INFO("Moving BarrettHand Spread: %.3f radians", req.radians);
    hand->trapezoidalMove(Hand::jp_type(req.radians), Hand::SPREAD, false);
    return true;
  }

//Function to control a BarrettHand Finger Velocity
template<size_t DOF>
  bool WamNode<DOF>::handFingerVel(wam_msgs::BHandFingerVel::Request &req, wam_msgs::BHandFingerVel::Response &res)
  {
    ROS_INFO("Moving BarrettHand Finger Velocities: %.3f, %.3f, %.3f m/s", req.velocity[0], req.velocity[1],
             req.velocity[2]);
    hand->velocityMove(Hand::jv_type(req.velocity[0], req.velocity[1], req.velocity[2], 0.0), Hand::GRASP);
    return true;
  }

//Function to control a BarrettHand Grasp Velocity
template<size_t DOF>
  bool WamNode<DOF>::handGraspVel(wam_msgs::BHandGraspVel::Request &req, wam_msgs::BHandGraspVel::Response &res)
  {
    ROS_INFO("Moving BarrettHand Grasp: %.3f m/s", req.velocity);
    hand->velocityMove(Hand::jv_type(req.velocity), Hand::GRASP);
    return true;
  }

//Function to control a BarrettHand Spread Velocity
template<size_t DOF>
  bool WamNode<DOF>::handSpreadVel(wam_msgs::BHandSpreadVel::Request &req, wam_msgs::BHandSpreadVel::Response &res)
  {
    ROS_INFO("Moving BarrettHand Spread: %.3f m/s", req.velocity);
    usleep(5000);
    hand->velocityMove(Hand::jv_type(req.velocity), Hand::SPREAD);
    return true;
  }

//Callback function for RT Cartesian Velocity messages
template<size_t DOF>
  void WamNode<DOF>::cartVelCB(const wam_msgs::RTCartVel::ConstPtr& msg)
  {
    if (cart_vel_status)
    {
      for (size_t i = 0; i < 3; i++)
        rt_cv_cmd[i] = msg->direction[i];
      new_rt_cmd = true;
      if (msg->magnitude != 0)
        cart_vel_mag = msg->magnitude;
    }
    last_cart_vel_msg_time = ros::Time::now();

  }

//Callback function for RT Orientation Velocity Messages
template<size_t DOF>
  void WamNode<DOF>::ortnVelCB(const wam_msgs::RTOrtnVel::ConstPtr& msg)
  {
    if (ortn_vel_status)
    {
      for (size_t i = 0; i < 3; i++)
        rt_ortn_cmd[i] = msg->angular[i];
      new_rt_cmd = true;
      if (msg->magnitude != 0)
        ortn_vel_mag = msg->magnitude;
    }
    last_ortn_vel_msg_time = ros::Time::now();
  }

//Callback function for RT Joint Velocity Messages
template<size_t DOF>
  void WamNode<DOF>::jntVelCB(const wam_msgs::RTJointVel::ConstPtr& msg)
  {
    if (msg->velocities.size() != DOF)
    {
      ROS_INFO("Commanded Joint Velocities != DOF of WAM");
      return;
    }
    if (jnt_vel_status)
    {
      for (size_t i = 0; i < DOF; i++)
        rt_jv_cmd[i] = msg->velocities[i];
      new_rt_cmd = true;
    }
    last_jnt_vel_msg_time = ros::Time::now();
  }

//Callback function for RT Joint Position Messages
template<size_t DOF>
  void WamNode<DOF>::jntPosCB(const wam_msgs::RTJointPos::ConstPtr& msg)
  {
    ROS_INFO("******Real-Time Joint Position Message Received!*******");
    if (msg->joints.size() != DOF)
    {
      ROS_INFO("Commanded Joint Positions != DOF of WAM");
      return;
    }
    ROS_INFO_STREAM("Joint Position Status: " << jnt_pos_status << " !");
    if (jnt_pos_status)
    {
      for (size_t i = 0; i < DOF; i++)
      {
        rt_jp_cmd[i] = msg->joints[i];
        rt_jp_rl[i] = msg->rate_limits[i];
      }
      ROS_INFO("******Set Up New Real Time Command!******");
      new_rt_cmd = true;
    }
    last_jnt_pos_msg_time = ros::Time::now();
    ROS_INFO_STREAM("******Current Time Stamp is " << last_jnt_pos_msg_time.toSec() << " !******" );
  }

//Callback function for RT Cartesian Position Messages
template<size_t DOF>
  void WamNode<DOF>::cartPosCB(const wam_msgs::RTCartPos::ConstPtr& msg)
  {
    if (cart_pos_status)
    {
      for (size_t i = 0; i < 3; i++)
      {
        rt_cp_cmd[i] = msg->position[i];
        rt_cp_rl[i] = msg->rate_limits[i];
      }
      new_rt_cmd = true;
    }
    last_cart_pos_msg_time = ros::Time::now();
  }

//Function to update the WAM publisher
template<size_t DOF>
  void WamNode<DOF>::publishWam()
  {
    while (ros::ok())
    {
        //biotac_sensors::BioTacHand bt_hand_msg;
        //bt_hand_msg = LEFT_HAND.collectBatch();

        //Use the system::output to get 
        //Current values to be publishedROS_INFO("***New Joint Position Command! Move In Joint Space with RealTime!***");
        //ROS_INFO("Number of subscribers for /joint_states topic: %i", wam_joint_state_pub.getNumSubscribers());
        jp_type jp = wam.getJointPositions();
        jt_type jt = wam.getJointTorques();
        jv_type jv = wam.getJointVelocities();
        cp_type cp_pub = wam.getToolPosition();
        Eigen::Quaterniond to_pub = wam.getToolOrientation();

        //publishing sensor_msgs/JointState to wam/joint_states
        for (size_t i = 0; i < DOF; i++)
        {
            wam_joint_state.position[i] = jp[i];
            wam_joint_state.velocity[i] = jv[i];
            wam_joint_state.effort[i] = jt[i];
        }
        wam_joint_state.header.stamp = ros::Time::now();
        wam_joint_state_pub.publish(wam_joint_state);

        // publish the joint states and biotac hand together
        //wam_joint_state_and_biotac.joint_state = wam_joint_state;
        //wam_joint_state_and_biotac.biotac_hand = bt_hand_msg;
        //wam_joint_state_and_biotac_pub.publish(wam_joint_state_and_biotac);

        //publishing geometry_msgs/PoseStamed to wam/pose
        wam_pose.header.stamp = ros::Time::now();
        wam_pose.pose.position.x = cp_pub[0];
        wam_pose.pose.position.y = cp_pub[1];
        wam_pose.pose.position.z = cp_pub[2];
        wam_pose.pose.orientation.w = to_pub.w();
        wam_pose.pose.orientation.x = to_pub.x();
        wam_pose.pose.orientation.y = to_pub.y();
        wam_pose.pose.orientation.z = to_pub.z();
        wam_pose_pub.publish(wam_pose);

        
        // publish the tf tranformation 
        //transform_.setOrigin(tf::Vector3(0.102, -0.715, 1.037));
        //tf::Quaternion q;
        //q.setRPY(3.1415926535897931, 0, 0);
        //transform_.setRotation(q);
        //br_.sendTransform(tf::StampedTransform(transform_, ros::Time::now(), "left_wam/base_link", "camera_rgb_optical_frame"));
        
        btsleep(1.0 / PUBLISH_FREQ);
    }
  }

//Function to update the real-time control loops
template<size_t DOF>
  void WamNode<DOF>::publishHand() //systems::PeriodicDataLogger<debug_tuple>& logger
  {
    while (ros::ok())
    {
      hand->update(); // Update the hand sensors
      Hand::jp_type hi = hand->getInnerLinkPosition(); // get finger positions information
      Hand::jp_type ho = hand->getOuterLinkPosition();
      for (size_t i = 0; i < 4; i++) // Save finger positions
        bhand_joint_state.position[i] = hi[i];
      for (size_t j = 0; j < 3; j++)
        bhand_joint_state.position[j + 4] = ho[j];
      bhand_joint_state.header.stamp = ros::Time::now(); // Set the timestamp
      bhand_joint_state_pub.publish(bhand_joint_state); // Publish the BarrettHand joint states
      btsleep(1.0 / BHAND_PUBLISH_FREQ); // Sleep according to the specified publishing frequency
    }
  }

//Function to update the real-time control loops
template<size_t DOF>
  void WamNode<DOF>::updateRT(ProductManager& pm) //systems::PeriodicDataLogger<debug_tuple>& logger
  {
    ROS_INFO_STREAM("cart_vel_status: " << cart_vel_status << " ortn_vel_status: " << ortn_vel_status << " jnt_vel_status: " << jnt_vel_status << " jnt_pos_status: " << jnt_pos_status << " cart_pos_status: " << cart_pos_status << " ortn_pos_status: " << ortn_pos_status);
    //ROS_INFO("***Update in Real Time!***");
    //Real-Time Cartesian Velocity Control Portion
    if (last_cart_vel_msg_time + rt_msg_timeout > ros::Time::now()) // checking if a cartesian velocity message has been published and if it is within timeout
    {
      if (!cart_vel_status)
      {
        cart_dir.setValue(cp_type(0.0, 0.0, 0.0)); // zeroing the cartesian direction
        current_cart_pos.setValue(wam.getToolPosition()); // Initializing the cartesian position
        current_ortn.setValue(wam.getToolOrientation()); // Initializing the orientation
        systems::forceConnect(ramp.output, mult_linear.input1); // connecting the ramp to multiplier
        systems::forceConnect(cart_dir.output, mult_linear.input2); // connecting the direction to the multiplier
        systems::forceConnect(mult_linear.output, cart_pos_sum.getInput(0)); // adding the output of the multiplier
        systems::forceConnect(current_cart_pos.output, cart_pos_sum.getInput(1)); // with the starting cartesian position offset
        systems::forceConnect(cart_pos_sum.output, rt_pose_cmd.getInput<0>()); // saving summed position as new commanded pose.position
        systems::forceConnect(current_ortn.output, rt_pose_cmd.getInput<1>()); // saving the original orientation to the pose.orientation
        ramp.setSlope(cart_vel_mag); // setting the slope to the commanded magnitude
        ramp.stop(); // ramp is stopped on startup
        ramp.setOutput(0.0); // ramp is re-zeroed on startup
        ramp.start(); // start the ramp
        wam.trackReferenceSignal(rt_pose_cmd.output); // command WAM to track the RT commanded (500 Hz) updated pose
      }
      else if (new_rt_cmd)
      {
        ramp.reset(); // reset the ramp to 0
        ramp.setSlope(cart_vel_mag);
        cart_dir.setValue(rt_cv_cmd); // set our cartesian direction to subscribed command
        current_cart_pos.setValue(wam.tpoTpController.referenceInput.getValue()); // updating the current position to the actual low level commanded value
      }
      cart_vel_status = true;
      new_rt_cmd = false;
    }

    //Real-Time Angular Velocity Control Portion
    else if (last_ortn_vel_msg_time + rt_msg_timeout > ros::Time::now()) // checking if a orientation velocity message has been published and if it is within timeout
    {
      if (!ortn_vel_status)
      {
        rpy_cmd.setValue(math::Vector<3>::type(0.0, 0.0, 0.0)); // zeroing the rpy command
        current_cart_pos.setValue(wam.getToolPosition()); // Initializing the cartesian position
        current_rpy_ortn.setValue(toRPY(wam.getToolOrientation())); // Initializing the orientation

        systems::forceConnect(ramp.output, mult_angular.input1); // connecting the ramp to multiplier
        systems::forceConnect(rpy_cmd.output, mult_angular.input2); // connecting the rpy command to the multiplier
        systems::forceConnect(mult_angular.output, ortn_cmd_sum.getInput(0)); // adding the output of the multiplier
        systems::forceConnect(current_rpy_ortn.output, ortn_cmd_sum.getInput(1)); // with the starting rpy orientation offset
        systems::forceConnect(ortn_cmd_sum.output, to_quat.input);
        systems::forceConnect(current_cart_pos.output, rt_pose_cmd.getInput<0>()); // saving the original position to the pose.position
        systems::forceConnect(to_quat.output, rt_pose_cmd.getInput<1>()); // saving the summed and converted new quaternion commmand as the pose.orientation
        ramp.setSlope(ortn_vel_mag); // setting the slope to the commanded magnitude
        ramp.stop(); // ramp is stopped on startup
        ramp.setOutput(0.0); // ramp is re-zeroed on startup
        ramp.start(); // start the ramp
        wam.trackReferenceSignal(rt_pose_cmd.output); // command the WAM to track the RT commanded up to (500 Hz) cartesian velocity
      }
      else if (new_rt_cmd)
      {
        ramp.reset(); // reset the ramp to 0
        ramp.setSlope(ortn_vel_mag); // updating the commanded angular velocity magnitude
        rpy_cmd.setValue(rt_ortn_cmd); // set our angular rpy command to subscribed command
        current_rpy_ortn.setValue(toRPY(wam.tpoToController.referenceInput.getValue())); // updating the current orientation to the actual low level commanded value
      }
      ortn_vel_status = true;
      new_rt_cmd = false;
    }

    //Real-Time Joint Velocity Control Portion
    else if (last_jnt_vel_msg_time + rt_msg_timeout > ros::Time::now()) // checking if a joint velocity message has been published and if it is within timeout
    {
      if (!jnt_vel_status)
      {
        jv_type jv_start;
        for (size_t i = 0; i < DOF; i++)
          jv_start[i] = 0.0;
        jv_track.setValue(jv_start); // zeroing the joint velocity command
        wam.trackReferenceSignal(jv_track.output); // command the WAM to track the RT commanded up to (500 Hz) joint velocities
      }
      else if (new_rt_cmd)
      {
        jv_track.setValue(rt_jv_cmd); // set our joint velocity to subscribed command
      }
      jnt_vel_status = true;
      new_rt_cmd = false;
    }
    
    //Real-Time Joint Position Control Portion
    else if (last_jnt_pos_msg_time + rt_msg_timeout > ros::Time::now()) // checking if a joint position message has been published and if it is within timeout
    {
      if (!jnt_pos_status)
      {
        ROS_INFO("***No New Joint Position Command! Move In Joint Space with RealTime!***");
        jp_type jp_start = wam.getJointPositions();
        jp_track.setValue(jp_start); // setting initial the joint position command
        jp_rl.setLimit(rt_jp_rl);
        systems::forceConnect(jp_track.output, jp_rl.input);
        wam.trackReferenceSignal(jp_rl.output); // command the WAM to track the RT commanded up to (500 Hz) joint positions
      }
      else if (new_rt_cmd)
      {
        ROS_INFO("***New Joint Position Command! Move In Joint Space with RealTime!***");
        jp_track.setValue(rt_jp_cmd); // set our joint position to subscribed command
        jp_rl.setLimit(rt_jp_rl); // set our rate limit to subscribed rate to control the rate of the moves
        systems::forceConnect(jp_track.output, jp_rl.input);
        wam.trackReferenceSignal(jp_rl.output); // command the WAM to track the RT commanded up to (500 Hz) joint positions
      }
      jnt_pos_status = true;
      new_rt_cmd = false;
    }
    
    
    


    else if (last_cart_pos_msg_time + rt_msg_timeout > ros::Time::now()) // checking if a cartesian position message has been published and if it is within timeout
    {
      if (!cart_pos_status)
      {
        cp_track.setValue(wam.getToolPosition());
        current_ortn.setValue(wam.getToolOrientation()); // Initializing the orientation
        cp_rl.setLimit(rt_cp_rl);
        systems::forceConnect(cp_track.output, cp_rl.input);
        systems::forceConnect(cp_rl.output, rt_pose_cmd.getInput<0>()); // saving the rate limited cartesian position command to the pose.position
        systems::forceConnect(current_ortn.output, rt_pose_cmd.getInput<1>()); // saving the original orientation to the pose.orientation
        wam.trackReferenceSignal(rt_pose_cmd.output); //Commanding the WAM to track the real-time pose command.
      }
      else if (new_rt_cmd)
      {
        cp_track.setValue(rt_cp_cmd); // Set our cartesian positions to subscribed command
        cp_rl.setLimit(rt_cp_rl); // Updating the rate limit to subscribed rate to control the rate of the moves
      }
      cart_pos_status = true;
      new_rt_cmd = false;
    }

    //If we fall out of 'Real-Time', hold joint positions
    else if (cart_vel_status | ortn_vel_status | jnt_vel_status | jnt_pos_status | cart_pos_status)
    {
      ROS_INFO("***system out of real time!***");
      wam.moveTo(wam.getJointPositions()); // Holds current joint positions upon a RT message timeout
      cart_vel_status = ortn_vel_status = jnt_vel_status = jnt_pos_status = cart_pos_status = ortn_pos_status = false;
    }
    /*
    else
    {
        ROS_INFO_STREAM("Fall out of real time: " << (cart_vel_status | ortn_vel_status | jnt_vel_status | jnt_pos_status | cart_pos_status) << "!");
        //Real-Time Cartesian Position Control Portion
        ROS_INFO_STREAM("Last Message Time: " << last_jnt_pos_msg_time.toSec());
        ROS_INFO_STREAM("Timeout Constant: " << rt_msg_timeout.toSec());
        ROS_INFO_STREAM("Time lapse: " << (last_jnt_pos_msg_time.toSec() + rt_msg_timeout.toSec() - ros::Time::now().toSec()));
        ROS_INFO("***messages outdated!***");
    }
    */
  }

//wam_main Function
template<size_t DOF>
  int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam)
  {
    BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
    
    //ProductManager pm_actual("/etc/barrett/bus1/default.conf");
    ros::init(argc, argv, "wam_node_l");
    //ccout << "a" << endl;
    WamNode<DOF> wam_node(wam);
    wam_node.init(pm);
    ros::Rate pub_rate(PUBLISH_FREQ);
  

    if (pm.getHand())
      boost::thread handPubThread(&WamNode<DOF>::publishHand, &wam_node);

    boost::thread WamPubThread(&WamNode<DOF>::publishWam, &wam_node);

    while (ros::ok()) //&& pm.getSafetyModule()->getMode() == SafetyModule::ACTIVE)
    {
      ros::spinOnce();
      //wam_node.publishWam(pm);
      wam_node.updateRT(pm);
      pub_rate.sleep();
    }

    return 0;
  }

