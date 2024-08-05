#!/usr/local/bin/python

import os

datadir = '/p/lustre1/hardy27/simulations/XeNeuMigdal_20240724/DD_10deg/'

execdir = "/g/g92/hardy27/BACCARAT/AnalysisTools/"
macro = "/g/g92/hardy27/BACCARAT/AnalysisTools/XeNeuReductionMacroTemplate.C"
base = "DD_14det_95cmXDistance_10deg_Migdal_"

files = os.listdir(datadir)

binfiles = [f for f in files if (f.endswith('ls_hit.root') and not f.endswith('reduced.root'))]

numfiles = len(binfiles)
print('{} bin files to process...'.format(numfiles))

i=0
print(datadir)

for i,f in enumerate(binfiles):
	#print(f)
	#if i > 1: break
	#print(f)
	basename = base + '{}'.format(i) 
	
	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )

	filetitle = f.split('.')[0]
	 
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 00:10:00\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=cahardy@stanford.edu --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
		"cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		"root -l -q \'" + macro + "(\"" + datadir + filetitle + \
		"\",\"" + datadir+filetitle+"_reduced.root\")\'" + "\n" + \
		"cd $OLDPWD \n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"
	
	scriptfile = open( scriptfilename, 'w' )
	scriptfile.write( thescript )
	scriptfile.close()
	
	os.system( "sbatch " + scriptfilename )

