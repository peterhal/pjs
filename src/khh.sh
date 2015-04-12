#! /bin/bash

ps -ef | egrep hh_server | cut -d ' ' -f 5 | xargs kill -9

