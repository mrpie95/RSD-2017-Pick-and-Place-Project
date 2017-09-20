%% Link parameters Definitions
% Here we define each link to roughly copy the linkage of ABB IRB120
% The robot was scaled down to in order to be able to clearly see the
% joints axis (However we could have changed the scale when changing the
% plot options.

% [Body rotation, up&dwn 1st link, up&down 2nd link, rotate smaller section, wrist movement, finer rotation] 
Link1 = RevoluteMDH('d', 0.2, 'a', 0, 'alpha', 0);
Link2 = RevoluteMDH('d', 0, 'a', 0, 'alpha', -pi/2, 'offset', -pi/2);
Link3 = RevoluteMDH('d', 0, 'a', 0.29, 'alpha', 0);
Link4 = RevoluteMDH('d', 0.2, 'a', 0, 'alpha', -pi/2);
Link5 = RevoluteMDH('d', 0, 'a', 0, 'alpha', -pi/2, 'offset', pi/2);
Link6 = RevoluteMDH('d', 0.09, 'a', 0, 'alpha', -pi/2, 'offset', -pi);


%% Set up the manipulator
ABB120 = SerialLink([Link1 Link2 Link3 Link4 Link5 Link6], 'name', 'ABB IRB120');

%% Define some poses
% Robot arm in the initial state with all links at their 0 positions
p0 = [0,  0,  0, 0, 0, 0];

% Robot arm's positions
% [Body rotation, up&dwn 1st link, up&down 2nd link, rotate smaller section, wrist movement, finer rotation] 

% p1 is the home position, which mimics the position when the robot goes out of the webcam frame to trigger the next sequence
% p2 hover over the block in the 2nd position on the grid
% p3 goes down and picks up the block at the 2nd position
% p4 hovers the block over to the right hand stack
% p5 Places the block down in the right hand stack
p1 = [pi/4,   pi/5,      0,  0, pi/5, pi/4];
p2 = [   0,   pi/5,  -pi/7,  0, pi/12,   0];
p3 = [   0, pi/2.9, -pi/10,  0, pi/4,    0];
p4 = [pi/5,  pi/10,   pi/9,  0, pi/5, pi/5];
p5 = [pi/5,   pi/4,   pi/4,  0, pi/2, pi/5];


%% Find the forward kinematics of each pose
fk_p1 = ABB120.fkine(p1);
fk_p2 = ABB120.fkine(p2);
fk_p3 = ABB120.fkine(p3);
fk_p4 = ABB120.fkine(p4);
fk_p5 = ABB120.fkine(p5);

%% Find the inverse kinematics for each position

% Since there are 6 linkages, we need a mask with 6 ones.
ik_ps = ABB120.ikine(fk_ps, p1, 'mask',[1 1 1 1 1 1 ]);
ik_p1 = ABB120.ikine(fk_p1, p0, 'mask',[1 1 1 1 1 1 ]);
ik_p2 = ABB120.ikine(fk_p2, p0, 'mask',[1 1 1 1 1 1 ]);
ik_p3 = ABB120.ikine(fk_p3, p0, 'mask',[1 1 1 1 1 1 ]);
ik_p4 = ABB120.ikine(fk_p4, p0, 'mask',[1 1 1 1 1 1 ]);
ik_p5 = ABB120.ikine(fk_p5, p0, 'mask',[1 1 1 1 1 1 ]);


%% Find the joint trajectory for ten steps between the two poses

% Set the number of points between each positon.
num_points = 20;

% This is the sequence of the movement, j12 is moving from position 1 to positon 2 and etc.
j12 = jtraj(ik_p1,ik_p2,num_points);
j23 = jtraj(ik_p2,ik_p3,num_points);
j32 = jtraj(ik_p3,ik_p2,num_points);
j24 = jtraj(ik_p2,ik_p4,num_points);
j45 = jtraj(ik_p4,ik_p5,num_points);
j54 = jtraj(ik_p5,ik_p4,num_points);
j41 = jtraj(ik_p4,ik_p1,num_points);

%% Show animation of the movement

% Set the plot options for the robot.
% Workspace: This line is required for robots with prismatic joints. The 
%   program can automatically determine the workspace required for rotation
%   only robots.
% Scale: Required to avoid errors - I recommend setting it to 1
% Reach: Required to avoid errors - I recommend setting it to 1
% Delay: Adds a delay between each iteration of the movement. This allows
%   you to slow down the movement to see what it happening

ABB120.plotopt = {'workspace',[-1, 1, -1, 1, 0, 1], ...
                       'scale', 1, 'reach',1, 'delay', 0.05};

% Plot the animation
ABB120.plot(j12);
ABB120.plot(j23);
ABB120.plot(j32);
ABB120.plot(j24);
ABB120.plot(j45);
ABB120.plot(j54);
ABB120.plot(j41);
