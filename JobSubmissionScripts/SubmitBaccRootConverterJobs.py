#!/usr/local/bin/python

import os
execdir = "/g/g92/hardy27/BACCARAT/tools"
datadir = "/p/lustre1/hardy27/simulations/XeNeuMigdal_20240724/DT_noSpread/"
executable = "/g/g92/hardy27/BACCARAT/tools/BaccRootConverter"
base = "XeNeuMigdal_rootified_"

files = os.listdir(datadir)
binfiles = [f for f in files if f.endswith('bin')]

numfiles = len(binfiles)
print('{} bin files to process...'.format(numfiles))


i=0

for f in binfiles:
	i=i+1
	basename = base + '{}'.format(i) 
	
	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 0:15:00\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=cahardy@stanford.edu --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
		"source ~/.profile.linux \n" + \
                "cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		executable + ' ' + datadir + f + "\n" + \
		"cd " + datadir + "\n" + \
		"chmod g+rwx *\n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"
	
	scriptfile = open( scriptfilename, 'w' )
	scriptfile.write( thescript )
	scriptfile.close()
	
	os.system( "sbatch " + scriptfilename )


