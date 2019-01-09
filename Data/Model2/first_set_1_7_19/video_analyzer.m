v = VideoReader('500_steps_100_hz_1_7_19_e.mp4');
%v = VideoReader('calibration_1_7_19.mp4');

numFrames = 0;
n = 0;
first_pt = 0;
second_pt = 0;
pts = zeros(2,2);
S = 'Ready for the second point';

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
                second_pt = 1;
                break;
            end
        end
    end
    disp(S)
    v.CurrentTime = v.Duration/10;
    n = 1;
    if (second_pt == 1)
        close(figure(1));
        break;
    end
end

pts

%close(figure(1));

%move = read(v);
%implay(move)

%mov = readFrame(v);

%v.CurrentTime = 4.8;
%mov2 = readFrame(v);

%result = mov-mov2-25;
%m = nnz(result);




