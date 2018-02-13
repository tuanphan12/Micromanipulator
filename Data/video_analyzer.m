v = VideoReader('a_2_100_steps_138_hz.avi');
numFrames = 0;
n = 0;
first_pt = 0;
second_pt = 0;
pts = zeros(2,2);

while 1
    while hasFrame(v)
        img = readFrame(v);
        figure(1),imshow(img,[]);

        if (first_pt == 0)
            [xi,yi,but] = ginput(1);
            if isequal(but,1)
                pts(1,1) = xi;
                pts(1,2) = yi;
                first_pt = 1;
            end
        elseif and((n == 1),(second_pt == 0))
            [xi,yi,but] = ginput(1);
            if isequal(but,97)
                ;
            elseif isequal(but,1)
                pts(2,1) = xi;
                pts(2,2) = yi;
                n = n + 1;
            end
        end
    end
    v.CurrentTime = v.Duration/2.0;
    n = 1;
end

%move = read(v);
%implay(move)

%mov = readFrame(v);

%v.CurrentTime = 4.8;
%mov2 = readFrame(v);

%result = mov-mov2-25;
%m = nnz(result);




