function pts = VideoPtsReader(v,n)
%where v is the VideoReader object and n is the number of points
pts = zeros(n,0);
numFrames = 0;
n = 1;
first_pt = 0;
second_pt = 0;

while hasFrame(v)
    img = readFrame(v);
    figure(1),imshow(img,[]);
    
    if (first_pt == 0)
        [xi,yi,but] = ginput(1);
        if isequal(but,1)
            first_pt = 1;
        end
    end
    
end