function redraw(frame, vidObj)
% REDRAW  Process a particular frame of the video
%   REDRAW(FRAME, VIDOBJ)
%       frame  - frame number to process
%       vidObj - VideoReader object

% Read frame
f = vidObj.read(frame);

% Get edge
f2 = edge(rgb2gray(f), 'canny');

% Overlay edge on original image
f3 = bsxfun(@plus, f,  uint8(255*f2));

% Display
image(f3); axis image off

end

vid = VideoReader('rhinos.avi');

% Set up video figure window
videofig(vid.NumberOfFrames, @(frm) redraw(frm, vid));

% Display initial frame
redraw(1, vid);