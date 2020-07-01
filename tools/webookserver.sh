#!/bin/bash
#
# LAST_UPDATE="20 May 2020 16:33"
#
# nano -c /home/webook/webookserver.sh
#
# Monit
# Ubuntu 12, CentOS 6 service
# service haproxy monit
# service haproxy monit
# service haproxy monit
#
# Archlinux, CentOS 7: systemctl
# sudo systemctl stop monit
# sudo systemctl restart monit
# sudo systemctl status monit
# 
# /home/webook/webookserver.sh start /home/webook/webook.wt 216.117.149.91 9696 /ww /home/webook/doc_root /home/webook/app_root /home/webook/logs
# /home/webook/webookserver.sh stop  /home/webook/webook.wt 216.117.149.91 9696 /ww /home/webook/doc_root /home/webook/app_root /home/webook/logs
#
# nano /etc/monit.conf
# Witty Wizard Setup
## Port:9696
#check process webook.wt-9696 with pidfile /home/webook/run/webook.wt-9696.pid
# start program = "/home/webook/webookserver.sh start /home/webook/webook.wt 216.117.149.91 9696"
# stop program  = "/home/webook/webookserver.sh stop  /home/webook/webook.wt 216.117.149.91 9696"
## Port:9686
#check process webook.wt-9686 with pidfile /home/webook/run/webook.wt-9686.pid
# start program = "/home/webook/webookserver.sh start /home/webook/webook.wt 216.117.149.91 9686"
# stop program  = "/home/webook/webookserver.sh stop  /home/webook/webook.wt 216.117.149.91 9686"
#
# HaProxy
# Ubuntu 12, CentOS 6 service
# service haproxy stop
# service haproxy restart
# service haproxy status
# 
# Archlinux, CentOS 7: systemctl
# sudo systemctl stop haproxy
# sudo systemctl restart haproxy
# sudo systemctl status haproxy
#
# nano /etc/haproxy/haproxy.cfg
#
#global
#    log 127.0.0.1 local0 
#    log 127.0.0.1 local1 notice
#    maxconn 4096
#    user haproxy
#    group haproxy
#    daemon
#defaults
#    log     global
#    mode    http
#    option  httplog
#    option  dontlognull
#    retries 3
#    option  redispatch
#    maxconn 1000
#    timeout connect 5000 # haproxy 1.5
#    timeout client 50000 # haproxy 1.5
#    timeout server 50000 # haproxy 1.5
#    option http-server-close
#    timeout http-keep-alive 3000
#    option forwardfor
#frontend wt
#    bind 216.117.149.91:80
#    # bind 0.0.0.0:80
#    option http-server-close
#    timeout http-keep-alive 3000
#    reqidel ^Client-IP:.*
#    reqidel ^X-Forwarded-For:.*
#    option forwardfor
#    # Set inside Witty Wizard main.cpp
#    acl has_path path /
#    reqirep ^([^\ :]*)\ /(.*) \1\ /ww/\2 if has_path
#    redirect prefix http://webook.org code 301     if { hdr(host) -i www.webook.org }
#    # Note: see webookserver.sh session-id-prefix
#    acl srv1 url_sub wtd=wt-9696
#    acl srv1_up nbsrv(bck1) gt 0
#    use_backend bck1 if srv1_up srv1
#    #
#    # Second Thread
#    # Note: see webookserver.sh session-id-prefix
#    # acl srv2  url_sub wtd=wt-9686
#    # acl srv2_up nbsrv(bck2) gt 0
#    # use_backend bck2 if srv2_up srv2 has_ww_uri
#    #
#    default_backend bck_lb
#    #
#backend bck_lb
#    balance roundrobin
#    server srv1 108.59.251.28:9696 track bck1/srv1
#    # server srv2 216.224.185.71:9696 track bck1/srv1
#backend bck1
#    balance roundrobin
#    server srv1 108.59.251.28:9696 check
#    # server srv2 216.224.185.71:9696 check
#backend bck2
#    balance roundrobin
#    server srv2 108.59.251.28:9686 check
# haproxy stat http://webook.org:8888/haproxy?stats
#listen stats
#    bind  :8888
#    mode  http
#    stats enable
#    stats hide-version
#    stats realm Haproxy\ Statistics
#    stats uri /
#    stats auth admin:admin
# #############################################################################
# ShowUsage
ShowUsage()
{
    printf "*******************************************\n";
    printf "WtHttp Server App Startup Program.\n";
    printf "Arguments:\n";
    printf "# 1 = Command: start | stop\n";
    printf "# 2 = Application full path and Name: /path/to/app.wt\n";
    printf "# 3 = IP Address: 123.123.123.123\n";
    printf "# 4 = Port Number: 9696\n";
    printf "# 5 = DeployPath: This becomes part of the URL: http://domain.com/Deploypath, if you make it '/', you will get an ugly URL '/?_='\n";
    printf "# 6 = DocRoot: /path/to/doc_root\n";
    printf "# 7 = AppRoot: /path/to/app_root\n";
    printf "# 8 = LogFiles: /path/to/Log-Files\n";
    printf "Arguments:           0 1     2               3          4    5          6       7       8\n";
    printf "Usage: webookserver.sh start /path/to/app.wt IP-Address Port DeployPath DocRoot AppRoot LogFiles\n";
    printf "Usage: webookserver.sh stop  /path/to/app.wt IP-Address Port DeployPath DocRoot AppRoot LogFiles\n";
    #                                               1     2                            3              4    5   6                     7                     8
    # start program = "/home/webook/webookserver.sh start /home/webook/WeBookServer.wb 216.117.149.91 9696 /ww /home/webook/doc_root /home/webook/app_root /home/webook/logs"

    printf "*******************************************\n";
    echo "webookserver.sh ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${8}";
    exit 1;
}
# #############################################################################
# Check Arguments
if [ "$#" -ne "8" ]; then ShowUsage; fi
# #############################################################################
# Declare All Varables
declare Action=$(basename "${1}");  # start or stop
declare ExeName=$(basename "${2}"); # Executable: app.wt
declare MyPath=$(dirname "${2}");   # Full Path:  /path/to
declare IpAddress="${3}";           # 123.123.123.123
declare Port="${4}";                # Port: 9696
declare DeployPath="${5}";          # This becomes part of the URL: http://domain.com/Deploypath, if you make it "/", you will get an ugly URL "/?_="
declare DocRoot="${6}";             # /path/to/doc_root
declare AppRoot="${7}";             # /path/to/app_root
declare LogFiles="${8}";            # Full Path to log files: /path/to/log-files
# #############################################################################
# Check App Existence
if [ ! -f "${MyPath}/${ExeName}" ]; then printf "Could not locate application: %s/%s\n" "${MyPath}" "${ExeName}"; ShowUsage; fi
# Check DocRoot Existence   
if [ ! -d "$DocRoot" ]; then printf "Could not locate DocRoot: %s\n" "$DocRoot"; ShowUsage; fi
# Check AppRoot Existence
if [ ! -d "$AppRoot" ]; then printf "Could not locate AppRoot: %s\n" "$AppRoot"; ShowUsage; fi
# Check run paths and create if does not exist
if [ ! -d "${MyPath}/run/" ]; then mkdir -p "${MyPath}/run/"; fi
if [ ! -d "${LogFiles}" ]; then mkdir -p "${LogFiles}"; fi
# #############################################################################
# KillPid
KillPid()
{
    printf "Checking for PID: %s/run/%s-%s.pid\n" "${MyPath}" "${ExeName}" "${Port}";
    if [ -f "${MyPath}/run/${ExeName}-${Port}.pid" ]; then
        pid=$(cat "${MyPath}/run/${ExeName}-${Port}.pid");
        if [[ "$pid" =~ ^[0-9]+$ ]] ; then
            printf "Killing pid %s\n" "$pid";
            kill "$pid";
            sleep 6;
            if [ -f "${MyPath}/run/${ExeName}-${Port}.pid" ]; then
                pid=$(cat "${MyPath}/run/${ExeName}-${Port}.pid");
                if [[ "$pid" =~ ^[0-9]+$ ]] ; then
                    kill -15 "$MyPid" # Be Gracefully
                    sleep 3;
                    kill -9 "$pid";   # By Force
                    sleep 1;
                fi
                if [ -f "${MyPath}/run/${ExeName}-${Port}.pid" ]; then
                    printf "Removing pid by force: %s/run/%s-%s.pid\n" "${MyPath}" "${ExeName}" "${Port}";
                    rm -f "${MyPath}/run/${ExeName}-${Port}.pid";
                fi
            fi
        fi
    fi
    # This is in case the pid does not exist but the program is running
    declare MyPids=$(pidof "$2");
    for MyPid in ${MyPids[@]}; do
        # We only want to kill the pid for this IP and port
        printf "Searching: %s-%s %s\n" "${ExeName}" "${MyPid}" "${IpAddress}\n";
        pgrep -f "${2} -p ${MyPath}/run/${ExeName}-${MyPid}.pid --docroot=$DocRoot --approot=$AppRoot --deploy-path=$DeployPath --http-address=$IpAddress --http-port=$MyPid --session-id-prefix=wt-$MyPid";
        if [[ "$?" ]]; then
            printf "kill %s\n" "$MyPid";
            kill "$MyPid";    # Kill it
            sleep 3;
            kill -15 "$MyPid" # Be Gracefully, if dead will return No such process
            sleep 3;
            kill -9  "$MyPid" # By Force, if dead will return No such process
        fi
    done
}
# #############################################################################
# Action
case "$Action" in
    'start')
        if [ -f "${MyPath}/run/${ExeName}-${Port}.pid" ]; then KillPid; fi
        touch "monittoken-${Port}";
        ulimit -s 1024;
        printf "Command: %s -p %s/run/%s-%s.pid --docroot=%s --approot=%s --deploy-path=%s --http-address=%s --http-port=%s --session-id-prefix=wt-%s\n" "$2" "${MyPath}" "${ExeName}" "${Port}" "$DocRoot" "$AppRoot" "$DeployPath" "$IpAddress" "$Port" "$Port";
        "$2" -p "${MyPath}/run/${ExeName}-${Port}.pid" "--docroot=$DocRoot" "--approot=$AppRoot" "--deploy-path=$DeployPath" "--http-address=$IpAddress" "--http-port=$Port" "--session-id-prefix=wt-$Port"  >> "${LogFiles}/${ExeName}-${Port}.log" 2>&1 &
        export jobpid=$!;
        sleep 1;
        printf "PID: %s\n" "$jobpid";
        ;;
    'stop')
        KillPid;
        ;;
    *)
        ShowUsage;
esac
# End of File #################################################################
