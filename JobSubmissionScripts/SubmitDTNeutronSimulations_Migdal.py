#!/usr/local/bin/python

import os
geant4 = "geant4.10.7.2"
root = "root_v6.24.08"
execdir = "/g/g92/hardy27/BACCARAT/"

macro = "/g/g92/hardy27/BACCARAT/XeNeuDT/Macros/DTNeutrons_10deg_95cmX_Migdal_EXAMPLE.mac"

datadir = '/p/lustre1/hardy27/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_EXAMPLE/'
base = "XeNeu_DT_"

for num in range(0,10):
	#if num > 0: break
	basename = base + str(num)

	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 0:10:00\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=cahardy@stanford.edu --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
                "source ~/.profile.linux \n" + \
                "cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		"./BACCARATExecutable " + macro + "\n" + \
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


