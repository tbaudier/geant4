#-------------------------------------------------------------------
# Last update: 12-Dec-2005
#
# This script should be run in the directory which has, as immediate
# subdirectories, the results of the Grid validation testing,
# which consist of tar-balls (one per subdirectory, each subdirectory
# being a job).
#
# The script does is following: it goes in each subdirectory, unpackes
# the tar-ball, keep a statistics of the files that should be contained
# in such tar-ball, and look if the jobs have started running but not
# terminated normally.
# At the end, it prints out the above information, both as overall
# statistics and also by calorimeter, by beam particle type, and
# by beam energy, and also produces the following files:
#   o  listDirFailed.txt : a list of the directories whose tar-ball
#                          do not have the expected files.
#   o  listDirRunCrashed.txt : a list of directories in which the
#                              Geant4 job(s) started running but
#                              did not terminated normally.
#   o  listPS.txt : a list of .ps files that should be examined.
#
# This script can be run also to get only the statistics, without
# unpacking the tar-balls: to do so, comment the line where
# ***LOOKHERE*** appears.
#
# This script has not input argument, and should be run as:
#         $  python unpack
#
#-------------------------------------------------------------------

import os
import sys
import string

# The following dictionaries keep track, calorimeter by calorimeter,
# beam particle by beam particle, and beam energy by beam energy,
# of the number of jobs which succeeded (i.e. there is the file
# "outputPvalues.log-..."), the corresponding fractions, the number
# of .PS files which are produced, and their fractions.

dictCalorimetersOK = { 'FeSci':0,
                       'CuSci':0,
                       'CuLAr':0,
                       'WLAr':0,
                       'PbSci':0,
                       'PbLAr':0,
                       'PbWO4':0 }

dictBeamParticlesOK = { 'e-':0,
                        'pi+':0,
                        'pi-':0,
                        'k+':0,
                        'k-':0,
                        'k0L':0,
                        'p':0,
                        'n':0 }

dictBeamEnergiesOK = { '1GeV':0,
                       '2GeV':0,
                       '3GeV':0,
                       '4GeV':0,
                       '5GeV':0,
                       '6GeV':0,
                       '7GeV':0,
                       '8GeV':0,
                       '9GeV':0,
                       '10GeV':0,
                       '20GeV':0,
                       '30GeV':0,
                       '50GeV':0,
                       '60GeV':0,
                       '80GeV':0,
                       '100GeV':0,
                       '120GeV':0,
                       '150GeV':0,
                       '180GeV':0,
                       '200GeV':0,
                       '250GeV':0,
                       '300GeV':0 }

dictCalorimetersPS = { 'FeSci':0,
                       'CuSci':0,
                       'CuLAr':0,
                       'WLAr':0,
                       'PbSci':0,
                       'PbLAr':0,
                       'PbWO4':0 }

dictBeamParticlesPS = { 'e-':0,
                        'pi+':0,
                        'pi-':0,
                        'k+':0,
                        'k-':0,
                        'k0L':0,
                        'p':0,
                        'n':0 }

dictBeamEnergiesPS = { '1GeV':0,
                       '2GeV':0,
                       '3GeV':0,
                       '4GeV':0,
                       '5GeV':0,
                       '6GeV':0,
                       '7GeV':0,
                       '8GeV':0,
                       '9GeV':0,
                       '10GeV':0,
                       '20GeV':0,
                       '30GeV':0,
                       '50GeV':0,
                       '60GeV':0,
                       '80GeV':0,
                       '100GeV':0,
                       '120GeV':0,
                       '150GeV':0,
                       '180GeV':0,
                       '200GeV':0,
                       '250GeV':0,
                       '300GeV':0 }

listDirFailed = open( "listDirFailed.txt", "w" )
listDirRunCrashed = open( "listDirRunCrashed.txt", "w" )
listPS = open( "listPS.txt", "w" )

os.system( "ls -1F | grep / > listDir.txt" )
listDir = open( "listDir.txt", "r" )

countDir = 0
countDirWithRunCrashes = 0
countDirWithNtuples = 0
countDirWithPvalues = 0
countDirWithPS = 0
countPS = 0

for dir in listDir :
    #print dir
    countDir += 1 
    saveDir = os.getcwd()
    os.chdir( dir.strip() )
    currentDir = os.getcwd()
    #print " I am in directory: ", currentDir

    os.system( "tar xvfz *.tgz" )       #***LOOKHERE**

    os.system( "ls -1 > listFiles.txt" )
    listFiles = open( "listFiles.txt", "r" )
    foundRunCrashed = 0
    foundNtuples = 0
    foundPvalues = 0
    foundPS = 0
    for iFile in listFiles :
        #print " iFile=", iFile
        if ( iFile.find( "output.log-" ) > -1 ) :
            # Look for runs that started but did not terminate.
            #print " --- Look at the output file = ", iFile.strip(), " --- "
            fileOutput = open( iFile.strip(), "r" )
            runStarted = 0
            runTerminated = 0
            for line in fileOutput :
                #print " line=", line.strip()
                if ( line.find( "Start Run processing." ) > -1 ) :
                     runStarted = 1
                     #print "  ***RUN STARTED*** : ", line.strip()
                if ( line.find( "Run terminated." ) > -1 ) :
                     runTerminated = 1
                     #print "  ***RUN TERMINATED*** : ", line.strip()
            if ( runStarted  and  ( not runTerminated ) ) :
                foundRunCrashed = 1
        if ( iFile.find( "ntuple.hbook" ) > -1 ) :
            foundNtuples = 1
        if ( iFile.find( "outputPvalues.log" ) > -1 ) :
            foundPvalues = 1
            for iDetector in dictCalorimetersOK.keys() :
                if ( iFile.find( "-" + iDetector + "-" ) > -1 ) :
                    dictCalorimetersOK[ iDetector ] += 1
            for iParticle in dictBeamParticlesOK.keys() :
                if ( iFile.find( "-" + iParticle + "-" ) > -1 ) :
                    dictBeamParticlesOK[ iParticle ] += 1
            for iEnergy in dictBeamEnergiesOK.keys() :
                if ( iFile.find( "-" + iEnergy + "-" ) > -1 ) :
                    dictBeamEnergiesOK[ iEnergy ] += 1
        if ( iFile.find( "plot.ps" ) > -1 ) :
            foundPS = 1
            countPS += 1
            listPS.write( currentDir + "/" + iFile )
            for iDetector in dictCalorimetersPS.keys() :
                if ( iFile.find( "-" + iDetector + "-" ) > -1 ) :
                    dictCalorimetersPS[ iDetector ] += 1
            for iParticle in dictBeamParticlesPS.keys() :
                if ( iFile.find( "-" + iParticle + "-" ) > -1 ) :
                    dictBeamParticlesPS[ iParticle ] += 1
            for iEnergy in dictBeamEnergiesPS.keys() :
                if ( iFile.find( "-" + iEnergy + "-" ) > -1 ) :
                    dictBeamEnergiesPS[ iEnergy ] += 1
    if ( foundRunCrashed ) :
        countDirWithRunCrashes += 1
        listDirRunCrashed.write( currentDir + "\n" )        
    if ( foundNtuples ) :
        countDirWithNtuples += 1
    if ( foundPvalues ) :
        countDirWithPvalues += 1
    if ( foundPS ) :
        countDirWithPS += 1
    if ( not foundNtuples  or  not foundPvalues ) :
        listDirFailed.write( currentDir + "\n" )

    listFiles.close()
    os.system( "rm listFiles.txt" )
    os.chdir( saveDir)

print " Summary: "
print " Number of directories = ", countDir
print " Number of directories with Run crashes = ", countDirWithRunCrashes
print " Number of directories with Ntuples = ", countDirWithNtuples
print " Number of directories with p-values = ", countDirWithPvalues
print " Number of directories with .PS = ", countDirWithPS
print " Number of .PS = ", countPS
print " "
print " Number of OK jobs and PS files for calorimeter types:"
for iDetector in dictCalorimetersOK.keys() :
    print "  calorimeter=", iDetector, \
          "  #OK=", dictCalorimetersOK[ iDetector ], " (", \
          int( dictCalorimetersOK[ iDetector ]*100.0 /
               float( countDirWithPvalues ) ), "%)", \
          "  #PS=", dictCalorimetersPS[ iDetector ], " (", \
          int( dictCalorimetersPS[ iDetector ]*100.0 /
               float( countPS ) ), "%)"
print " "
print " Number of OK jobs and PS files for beam particles:"
for iParticle in dictBeamParticlesOK.keys() :
    print "  particle=", iParticle, \
          "  #OK=", dictBeamParticlesOK[ iParticle ], " (", \
          int( dictBeamParticlesOK[ iParticle ]*100.0 /
               float( countDirWithPvalues ) ), "%)", \
          "  #PS=", dictBeamParticlesPS[ iParticle ], " (", \
          int( dictBeamParticlesPS[ iParticle ]*100.0 /
               float( countPS ) ), "%)"
print " "
print " Number of OK jobs and PS files for beam energies:"
for iEnergy in dictBeamEnergiesOK.keys() :
    print "  energy=", iEnergy, \
          "  #OK=", dictBeamEnergiesOK[ iEnergy ], " (", \
          int( dictBeamEnergiesOK[ iEnergy ]*100.0 /
               float( countDirWithPvalues ) ), "%)", \
          "  #PS=", dictBeamEnergiesPS[ iEnergy ], " (", \
          int( dictBeamEnergiesPS[ iEnergy ]*100.0 /
               float( countPS ) ), "%)"
print " "

listDir.close()
listDirFailed.close()
listDirRunCrashed.close()
listPS.close()


#-------------------------------------------------------------------

