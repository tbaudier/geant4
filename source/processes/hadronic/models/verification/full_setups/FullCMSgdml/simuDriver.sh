#!/bin/sh
#
#----------------------------------------------------------------------------
# This Bash shell script has the following 4 parameters:
#
#   1)  Geant4 reference; e.g. 9.1.p02 , or a local directory. 
#   2)  Physics List; e.g. LHEP
#   3) Number of Events; e.g. 5k
#   4) Bfield; e.g. 4.0tesla
#
# This script invokes the Python  build.py  that writes the setup
# and the Geant4 command file, and then it builds the application
# and runs it.
#
#----------------------------------------------------------------------------
#
echo ' ========== START simuDriver.sh ========== '
#
export REF=$1
export PHYSICS=$2
export EVENTS=$3
export BFIELD=$4
#
echo ' REF         =' $REF
echo ' PHYSICS     =' $PHYSICS
echo ' EVENTS      =' $EVENTS
echo ' BFIELD      =' $BFIELD
#
export LABEL=$REF-$PHYSICS-$EVENTS-B$BFIELD ;
###echo ' 1) LABEL=' $LABEL
python build.py $REF $PHYSICS $EVENTS $BFIELD ;
if [ $? != 0 ] ; then
    echo ' ***ERROR*** from: python build.py ... ! exitCode = 11' ;
    exit 11 ;
fi
. setup.sh ;
if [ $? != 0 ] ; then
    echo ' ***ERROR*** from: . setup.sh-... ! exitCode = 12' ; 
    exit 12 ;	
fi
#
echo '  ' ;
echo '--- Check platform / environment --- ' ;
echo '*** g++ -v ***' ;                    g++ -v                    ; echo ' ' ;
echo '*** which g++ ***' ;                 which g++                 ; echo ' ' ;
echo '*** uname -a ***' ;                  uname -a                  ; echo ' ' ;
echo '*** cat /etc/issue ***' ;            cat /etc/issue ;        
echo '*** cat /etc/cpuinfo ***' ;          cat /proc/cpuinfo ;
echo '*** DIR_INSTALLATIONS = '            $DIR_INSTALLATIONS        ; echo ' ' ;
echo '*** ls -lh $DIR_INSTALLATIONS ***' ; ls -lh $DIR_INSTALLATIONS ; echo ' ' ;
echo '*** G4INSTALL = '                    $G4INSTALL                ; echo ' ' ;
echo '*** ls -lh $G4INSTALL ***' ;         ls -lh $G4INSTALL         ; echo ' ' ;
echo '*** PWD = '                          $PWD                      ; echo ' ' ;
echo '*** ls -lh $PWD *** ' ;              ls -lh $PWD ;
echo '------------------------------------ ' ;    
echo '  ' ;
#
echo '  '; echo ' G4INSTALL = ' $G4INSTALL; echo ' running REF = ' $REF ; echo '  ' ;
rm -rf tmp/ ;
gmake ;
if [ $? != 0 ] ; then
	echo ' ***ERROR*** from: gmake ! exitCode = 13' ;  
	exit 13 ;	
fi    
mainApplication run.g4 > output.log-$LABEL 2>&1 ;
EXITCODE=$? ;
if [ $EXITCODE != 0 ] ; then
	echo ' ***ERROR*** from: mainApplication run.g4 ! exitCode = 14' ;  
	rm -rf tmp/ ;
	exit $EXITCODE ;
fi
#
echo ' ' ;
echo '--- Check results after running 1st reference ---' ; 
echo '*** ls -lth ***' ;  ls -lth ;
echo '-------------------------------------------------' ;
echo ' ' ;
#
if [[ $? != 0 ]] ; then
    echo ' ************************************************** ' ;
    echo ' ******       simuDriver.sh  FAILED!         ****** ' ;
    echo ' ************************************************** ' ;
    exit 69 ;
else
    echo ' ************************************************** ' ;
    echo ' ******       simuDriver.sh  OK!             ****** ' ;
    echo ' ************************************************** ' ;
fi
#
echo ' ========== END simuDriver.sh ========== '
