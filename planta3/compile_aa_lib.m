if isunix
    mex('AnalogIn.c','-ldl');
else
    mex('AnalogIn.c');
end

if isunix
    mex('AnalogOut.c','-ldl');
else
    mex('AnalogOut.c');
end