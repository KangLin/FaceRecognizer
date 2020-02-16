#!/bin/bash
set -ev

num=0
while [ $num -le 10 ] ;
    do sleep 300 ;
	   num=$(($num+1)) ;
           printf 'echo 5 min\r\n' ;
    done 
