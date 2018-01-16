% Input:
%   IMUOrientAngle(rad or quaternion): the tilt angles/orientation of the IMU frames
%   rotAngle(rad): a 4x1 vector containing the rotation angles of MCP(abduction/adduction), MCP(flexion/extention), PIP(flexion/extention), DIP(flexion/extention)
% Output:
%   fingerOrient_new: the orientations of the joint frames after moving
%   IMUOrient_new: the orientations of the IMU frames after moving

function [segOrient_new, IMUOrient_new] = configUpdate_OS(IMU_Placement, segMove)

%% Accept two types of input for the IMU orientation
if isa(IMU_Placement,'Quaternion')      % 1. quaternion
    IMU_Placement_quat = IMU_Placement;
else                                    % 2. angle-axis representation
    IMU_Placement_quat = angvec2q(IMU_Placement);      % Create a 1xn quaternion vector which contains the orientations of the IMUs' deviation (const states)
end

%% Accept two types of input for the finger movement
if isa(segMove, 'Quaternion')
    segMove_quat = segMove;
else
    segMove_quat = angvec2q(segMove);
end

%% Update
if length(IMU_Placement_quat) == 1
    segOrient_new = segMove_quat;
    IMUOrient_new = segOrient_new * IMU_Placement_quat;
else
    segOrient_new = segMove_quat / IMU_Placement_quat(1);
    IMUOrient_new = segOrient_new * IMU_Placement_quat(2);
end