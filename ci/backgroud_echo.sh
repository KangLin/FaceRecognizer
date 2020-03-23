#!/bin/bash
set -ev

num=0
while [ $num -le 10 ] ;
    do sleep 300 ;
	   num=$(($num+1)) ;
           echo '5 min\n' ;
    done 
