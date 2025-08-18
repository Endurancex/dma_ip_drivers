clear
clc

file_size_byte = 512*1024*1024; % 512MB

filename = './data/tx_data_512MB.bin';

if exist(filename, 'file')
    delete(filename);
end
% int32 type: 4bytes per number
numElements = file_size_byte / 4; 

data = uint32(0 : numElements-1);

fid = fopen(filename, 'w');

fwrite(fid, data, 'uint32');

fclose(fid);