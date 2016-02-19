function data = aprs2csv(filename,varargin)

if (nargin > 1)
    data = varargin{1};
end

% read the file
str = fileread(filename);

% determine the callsign
callsign = filename(1:findstr(filename,'.')-1);
callsign = strrep(callsign, '-', '');

% regex pattern to extract info
regex_pattern = ['(?<date>[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}).*?' ...
    'latitude: (?<lat>[+-]*\d*\.\d*).*?longitude: (?<lng>[+-]*\d*\.\d*).*?altitude: (?<alt>[+-]*\d*\.*\d*)'];

% extract the data
pkts = regexp(str,regex_pattern,'Names');

% initalize data
data.(callsign).lat = [];
data.(callsign).lng = [];
data.(callsign).alt = [];
data.(callsign).time = [];

% convert to arrays
for i=1:size(pkts,2)
    data.(callsign).lat = [data.(callsign).lat; str2double(pkts(i).lat)];
    data.(callsign).lng = [data.(callsign).lng; str2double(pkts(i).lng)];
    data.(callsign).alt = [data.(callsign).alt; str2double(pkts(i).alt)];
    data.(callsign).time = [data.(callsign).time; datenum(pkts(i).date)];
end

% [callsign '.csv']

% open file
fileID = fopen([callsign '.csv'],'w');
    
% write header
fprintf(fileID,'time, lat [deg], lng [deg], alt [m]\n');

% write data
for i=1:size(pkts,2)
    fprintf(fileID,'%s,%f,%f,%f\n', ...
        datestr(data.(callsign).time(i),'yyyy-mm-dd HH:MM:SS'),...
        data.(callsign).lat(i),...
        data.(callsign).lng(i),...
        data.(callsign).alt(i));
end

fclose(fileID);
% plot(data.lng,data.lat)
% figure()
% plot(data.time,data.alt)
% dynamicDateTicks()