#!/bin/bash
set -ev

num=0
while [ $num -le 10 ] ;
    do sleep 300 ;
	   num=$(($num+1)) ;
           printf 'y\r\n' ;
    done 
