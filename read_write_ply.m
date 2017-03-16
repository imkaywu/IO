function read_write_ply(ifname, ofname, P)
% Written by Chenxi cxliu@ucla.edu
% Input: fname: output file name, e.g. 'data.ply'
%        P: 3*m matrix with the rows indicating X, Y, Z
%        C: 3*m matrix with the rows indicating R, G, B

ifid = fopen(ifname);
ofid = fopen(ofname, 'w');

tline = fgets(ifid);
c = 0;
num = size(P, 1);
while ~feof(ifid)
    if c == 0
        fprintf(ofid, tline);
        if(strncmp(tline, 'end_header', 10))
            c = 1;
        end
    elseif(c <= num)
%         fprintf(ofid, '%f %f %f\n', P(c, 1), P(c, 2), P(c, 3));
        fprintf(ofid, '%f %f %f %f %f %f\n', P(c, 1), P(c, 2), P(c, 3), P(c, 4), P(c, 5), P(c, 6));
        c = c + 1;
    else
        c = 0;
    end
    tline = fgets(ifid);
end

fprintf(ofid, tline);
fprintf(ofid, tline);
fclose(ifid);
fclose(ofid);