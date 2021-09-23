HOW TO USE THE MACROS

There are two macros: qa_check.C and qa_test.C.
The first one determines if any of the indicated files
is corrupted and creates the list of good and bad ones.
The second one takes only the good files and creates QA plots.

There is an option to automate the entire process. For this 
purpose, a contr.sh script was created. It consists of 4 steps:

1. Create txt files for all data.
2. Check data for corrputed files
3. Create QA plots
4. Create html reports

In the first step, the script creates the lists 
of all data sets that need to be checked. By default, all data
is stored in /eos/nica/mpd/sim/data/MiniDst/. In case the location
is different, the line 35 of contr.sh needs to be changed.
The lists are written to txt files in directory dataacc 
and are named after their production names in MiniDst directory.

In the second step, qa_check.C macro takes the txt files created
in previous step and determines if the files listed there are
not corrupted. All of the correct files are listed in txt files
stored in datagood directory. Moreover, basic html reports
are created and stored in directory termed report.

In the third step, macro qa_test.C is executed. It takes the input
from goodfiles directory and writes root files to data directory.

In the fourth step, root files from data directory are converted
to html and stored in html directory. Then, all files there 
are packed using tar and written to the location of the script
and macros. 

ATTENTION!
In order for automation to work, several directories need to be
created in the location of the macros:

data
dataacc
datagood
html
report

In case any of the above is missing, use the following command:
mkdir data dataacc datagood html report

After that, script is ready to execute using the command:
source contr.sh
You will be asked if you want to execute any of the steps 
described above.

Some files from one production crash the macros so they make
it impossible to find them automatically. In order for macros 
to work properly they need to be removed manually from dataacc 
txt files.

/eos/nica/mpd/sim/data/MiniDst/dst-AuAu-11.5GeV-mp10-20-pwg3-1000ev/AuAu/11.5GeV-00.0_16.0fm/vHLLE_UrQMD/hydro-1PT-mp10-20-pwg3/AuAu_ecm11.5GeV_hydroON_EoS1PT_0-16fm_1000ev_11096-1.reco.MiniDst.root

/eos/nica/mpd/sim/data/MiniDst/dst-AuAu-11.5GeV-mp10-20-pwg3-1000ev/AuAu/11.5GeV-00.0_16.0fm/vHLLE_UrQMD/hydro-1PT-mp10-20-pwg3/AuAu_ecm11.5GeV_hydroON_EoS1PT_0-16fm_1000ev_11726-0.reco.MiniDst.root

/eos/nica/mpd/sim/data/MiniDst/dst-AuAu-11.5GeV-mp10-20-pwg3-1000ev/AuAu/11.5GeV-00.0_16.0fm/vHLLE_UrQMD/hydro-1PT-mp10-20-pwg3/AuAu_ecm11.5GeV_hydroON_EoS1PT_0-16fm_1000ev_14253-1.reco.MiniDst.root
