%% Define the link parameters for the manipulator
% In RME40003 we are using the "modified Denavit-Hartenberg convention. Use
% the RevoluteMDH and PrismaticMDH link definitions to accomodate this.
Link1 = RevoluteMDH('d', 0, 'a', 0, 'alpha', 0);
Link2 = RevoluteMDH('d', 0, 'a', 0, 'alpha', pi/2);
Link3 = RevoluteMDH('d', 0, 'a', 0.3, 'alpha', 0);
%Link 4 is incorrect, it needs to rotate along the y axis but not sure how
%to do that in this
Link4 = RevoluteMDH('d', 0, 'a', 0.1, 'alpha', 0);
Link5 = RevoluteMDH('d', 0, 'a', 0.1, 'alpha', pi/2);
Link6 = RevoluteMDH('d', 0, 'a', 0, 'alpha', pi/2);

%% Set up the manipulator
rsd_example = SerialLink([Link1 Link2 Link3 Link4 Link5], 'name', 'RSD Example');

%% Define some poses
% Robot arm pointing south fully extended, end gripper rotated 90 degrees
qs = [0, 0, 0, 0, 0]; 

% Robot arm pointing east end gripper rotated 180 degrees
qe = [pi/2, pi/4, -pi/2, 0, 0];

%% Find the forward kinematics of the pose
fk_qs = rsd_example.fkine(qs);
fk_qe = rsd_example.fkine(qe);

%% Find the inverse kinematics for fk_qs and fk_qe

% Calculate the inverse kinematics. Set a mask for the axes. For a two axis
% robot use [1 1 0 0 0 0]. For a three axis robot use [1 1 1 0 0 0] etc.
ik_qs = rsd_example.ikine(fk_qs, qe, 'mask',[1 1 1 1 1 0 ]);
ik_qe = rsd_example.ikine(fk_qe, qs, 'mask',[1 1 1 1 1 0 ]);

%% Find the joint trajectory for ten steps between the two poses

% Set the number of points
num_points = 20;

% Moving from qs to qe
jt = jtraj(ik_qs,ik_qe,num_points);

%% Show animation of the movement

% Set the plot options for the robot.
% Workspace: This line is required for robots with prismatic joints. The 
%   program can automatically determine the workspace required for rotation
%   only robots.
% Scale: Required to avoid errors - I recommend setting it to 1
% Reach: Required to avoid errors - I recommend setting it to 1
% Delay: Adds a delay between each iteration of the movement. This allows
%   you to slow down the movement to see what it happening

rsd_example.plotopt = {'workspace',[-1, 1, -1, 1, -1, 1], ...
                       'scale', 1, 'reach',1, 'delay', 0.1};

% Plot the animation
rsd_example.plot(jt)
