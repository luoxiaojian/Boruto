mkdir('tasksets4');
homedir='tasksets4/';

for i=4:20
	dirname=strcat('u_', int2str(i*5));
	dirname=strcat(dirname, '/');
	dirname=strcat(homedir, dirname);
	mkdir(dirname);
	generation(i, 10, i*5, 10000, dirname);
end

disp('generation finished');
