#!/bin/bash
#
# Written by Jeffrey Scott Flesher
# This Bash Script is used to Install, Update, and Build WeBookServer
#
# Last Update: 20 May 2020
#
# These are specific to VPS account that you can edit
declare MyDefaultRootFolder;    MyDefaultRootFolder     = "/home/webook";
declare MyDefaultWeBookFolder;  MyDefaultWeBookFolder   = "examples/httpserver/webookserver";
declare MyBuildRoot;            MyBuildRoot             = "build/WeBook";
declare MyRootFolder;           MyRootFolder            = "$HOME";
declare MyQHttpServerRepo;      MyQHttpServerRepo       = "https://github.com/qt-labs/qthttpserver.git";
declare MyWeBookServerRepo;     MyWeBookServerRepo      = "https://github.com/Light-Wizzard/WeBookServer.git";
declare MyBuildPath;
declare -x MyWeBookServerFullPathExecutable;
declare -i MyLastCommandPassed; MyLastCommandPassed = 0; # 1 = False and 0 = True, think how many Errors
declare MyWeBookFolderInstallerIntoExamples; 
###############################################################################
#
# 1->(New Users Password) only used for Switch Install, otherwise use "NO-PASSWORD"
# 2->(Salt for Password)  only used for Switch Install, otherwise use "NO-SALT"
# 3->(Switch:Install,Update,Build)
# webooksetup.sh "PASSWORD"    "SALT"    Install;
# webooksetup.sh "NO-PASSWORD" "NO-SALT" Update;
# webooksetup.sh "NO-PASSWORD" "NO-SALT" Build;
#
showUsage()
{
    echo "1->(New Users Password) only used for Switch Install, otherwise use NO-PASSWORD";
    echo "2->(Salt for Password)  only used for Switch Install, otherwise use NO-SALT";
    echo "3->(Switch:Install,Update,Build)";
    echo "webooksetup.sh PASSWORD    SALT    Install";
    echo "webooksetup.sh NO-PASSWORD NO-SALT Update";
    echo "webooksetup.sh NO-PASSWORD NO-SALT Build";
    echo "Note: if using Install, you cannot use NO-PASSWORD or NO-SALT"
    exit 1;
}
###############################################################################
[[ $# -ne 3 ]] && { echo "LOCALIZE_WRONG_ARGS_PASSED_TO Counted $# Parameters - Usage: ${FUNCNAME[0]}() 1->(New Users Password) 2->(Salt for Password) 3->(Switch:Install,Update,Build) $(basename "${BASH_SOURCE[0]}") -> ${FUNCNAME[0]}() : ${LINENO[0]} > ${FUNCNAME[1]}()"; showUsage; }
###############################################################################
# OS Info
declare -x My_OS;               My_OS="";                        # OS: Linux, FreeBSD, MAC, Windows, Sun JDS, Solaris/Sparc, sunbsd, HP-UX, IRIX, OSF1, ULTRIX, AIX 
declare -x My_DIST;             My_DIST="";                      # CentOS, Arch Linux, Debian, Redhat, Slackware, Suse, unitedlinux, Mandrake, Yellow dog, Gentoo
declare -x My_PSUEDONAME;       My_PSUEDONAME="";                # PSUEDONAME
declare -x My_ARCH;             My_ARCH="";                      # x86 or x86_64
declare -x My_Ver;              My_Ver="";                       # OS Version
declare -x My_Ver_Major;        My_Ver_Major="";                 # Major Version Number
declare -x My_Ver_Minor;        My_Ver_Minor="";                 # Major Version Number
declare -x My_OS_Update;        My_OS_Update="";                 # Patch Number
declare -x My_OS_Package;       My_OS_Package="";                #
declare MySupperSecretPassword; MySupperSecretPassword="${1}";   # encrypt decrypt 
declare MySalt;                 MySalt="${2}";                   # Salt for 
declare MySwitch;               MySwitch="${3}";                 # Install,Update
###############################################################################
###############################################################################
setPaths()
{
    # Never run any app under root
    if [ "$MyRootFolder" == "root" ]; then
        MyRootFolder = "$MyDefaultRootFolder";
    fi
    # Check to make sure Root Folder Exists
    if [ -d "$MyRootFolder" ]; then
        # /home/webook/build/WeBook
        MyBuildPath = "${MyRootFolder}/${MyBuildRoot}";
    else
        mkdir -p "${MyRootFolder}/${MyBuildRoot}";
    fi
    #
    if [ -d "${MyBuildPath}/${MyDefaultWeBookFolder}" ]; then
        MyWeBookFolderInstallerIntoExamples = "${MyBuildPath}/${MyDefaultWeBookFolder}"
    fi
    #
    # Check to see if webookserver exist, it might not
    if [ -f "${MyBuildPath}/${MyDefaultWeBookFolder}/webookserver" ]; then
        MyWeBookServerFullPathExecutable="${MyBuildPath}/${MyDefaultWeBookFolder}/webookserver";
        export MyWeBookServerFullPathExecutable;    
    fi
}
###############################################################################
###############################################################################
# 1: String to Encrypt/Decrypt
# 2: Password
# 3: encrypt or decrypt
passwordSafe()
{
    #local result;
    if [ "${3}" == "encrypt" ]; then
        #result = encrypt_str "${1}";
        echo -e "${1}" | openssl enc -aes-128-cbc -a -salt -pass pass:"${2}";
    else
        echo -e "${1}" | openssl enc -aes-128-cbc -a -d -salt -pass pass:"${2}";
        #result = encrypt_str "${1}";
    fi
}
###############################################################################
###############################################################################
testPasswordSafe()
{
    # Note: ! cannot be in String unless its slashed \!
    local thisString; thisString='Abc123456*@#$%^&d|Password';
    local -a thisReturnStatus; thisReturnStatus=$(password_safe "${thisString}" "MyPassword" "encrypt" );
    if [[ $(password_safe "$thisReturnStatus" "MyPassword" "decrypt" ) == "${thisString}" ]]; then
        echo "LOCALIZE_TEST_FUNCTION_PASSED $(sub_string "${FUNCNAME[0]}" "test_" 2)():${LINENO[0]}";
        MyLastCommandPassed = 0;
    else
        echo "LOCALIZE_TEST_FUNCTION_FAILED $(sub_string "${FUNCNAME[0]}" "test_" 2)() : ${LINENO[0]}";
        MyLastCommandPassed = 1;
    fi
}
################################ END setPaths #################################
###############################################################################
is_user()
{
    #egrep -i "^${1}" /etc/passwd > /dev/null 2>&1;
    grep -E ^"${1}" "/etc/passwd" > /dev/null 2>&1;
    return "${?}";
}
###############################################################################
###############################################################################
is_installed()
{
    if [ $# -ne 4 ]; then echo "LOCALIZE_WRONG_ARGS_PASSED_TO ${FUNCNAME[0]} @ $(basename "${BASH_SOURCE[0]}") -> ${FUNCNAME[0]}() : ${LINENO[0]} @ ${FUNCNAME[1]}"; exit 1; fi

    if hash "${1}" 2>/dev/null; then
        return 0;
    else
        if [ "${3}" -eq 1 ]; then
            install_package_with "${2}" 0 1 "${4}";
            if hash "${1}" 2>/dev/null; then
                return 0;
            else
                return 1;
            fi
        else
            return 1;
        fi
    fi
}
###############################################################################
###############################################################################
get_ip_from_url()
{
    if ! is_installed "host" "dnsutils" 1 0; then
        return 0;
    fi
    host "${1}" | awk '/^[[:alnum:].-]+ has address/ {print $4; exit }';
}
###############################################################################
###############################################################################
# 1->(Package)
# 2->(Confirm)
# 3->(Force)
# 4->(alternet install)
# install_package_with 
install_package_with()
{
    [[ $# -ne 4 ]] && { echo "LOCALIZE_WRONG_ARGS_PASSED_TO Counted $# Parameters - Usage: ${FUNCNAME[0]}() 1->(Package) 2->(Confirm) 3->(Force) 4->(alternet install) $(basename "${BASH_SOURCE[0]}") -> ${FUNCNAME[0]}() : ${LINENO[0]} > ${FUNCNAME[1]}()"; return 1; }
    if [[ -z ${My_DIST} ]]; then setOsInfo; fi
    if [ "$REFRESH_REPO" -eq 1 ]; then
        REFRESH_REPO=0;
        if [ "${My_DIST}" == "redhat" ]; then # -------------------------------- Redhat, Centos, Fedora
            yum update -y;
        elif [ "${My_DIST}" == "archlinux" ]; then # --------------------------- My_PSUEDONAME = Archlinux Distros
            pacman --noconfirm -Syu;
        elif [ "${My_DIST}" == "debian" ]; then # ------------------------------ Debian: My_PSUEDONAME = Ubuntu, LMDE - Distros
            apt-get update -y;
        elif [ "${My_DIST}" == "unitedlinux" ]; then # ------------------------- My_PSUEDONAME = unitedlinux Distros
            zypper update;
        elif [ "${My_DIST}" == "mandrake" ]; then # ---------------------------- My_PSUEDONAME = Mandrake Distros
            echo "LOCALIZE_DO_INSTALL_PACKAGE_WITH_UNSUPPORTED";
            # urpmi "${1}"
        elif [ "${My_DIST}" == "suse" ]; then # -------------------------------- My_PSUEDONAME = Suse Distros
            zypper update;
        fi
    fi
    #
    if [ "${My_DIST}" == "redhat" ]; then # -------------------------------- Redhat, Centos, Fedora
        if [ "${2}" -eq 0 ] && [ "$3" -eq 0 ]; then    # Confirm
            yum install "${1}";
        elif [ "${2}" -eq 0 ] && [ "$3" -eq 1 ]; then  # Confirm and Force
            rpm -e --nodeps PACKAGE
            # or rpm -e --justdb --nodeps PACKAGE
            yum install "${1}";
        elif [ "${2}" -eq 1 ] && [ "$3" -eq 1 ]; then  # No Confirm and Force
            rpm -e --nodeps PACKAGE
            # or rpm -e --justdb --nodeps PACKAGE
            yum install "${1}" -y;
        elif [ "${2}" -eq 1 ] && [ "$3" -eq 0 ]; then  # No Confirm
            yum install "${1}" -y;
        else                                       # Default No Confirm
            yum install "${1}" -y;
        fi
    elif [ "${My_DIST}" == "archlinux" ]; then # --------------------------- My_PSUEDONAME = Archlinux Distros
        if [ "${4}" -eq 0 ]; then    # Alternat Installer
            if [ "${2}" -eq 0 ] && [ "$3" -eq 0 ]; then    # Confirm
                pacman --needed -S "${1}";
            elif [ "${2}" -eq 0 ] && [ "$3" -eq 1 ]; then  # Confirm and Force
                pacman --needed --force -S "${1}";
            elif [ "${2}" -eq 1 ] && [ "$3" -eq 1 ]; then  # No Confirm and Force
                pacman --needed --noconfirm --force -S "${1}";
            elif [ "${2}" -eq 1 ] && [ "$3" -eq 0 ]; then  # No Confirm
                pacman --needed --noconfirm -S "${1}";
            else
                pacman --noconfirm --needed -S "${1}";   # Default No Confirm
            fi
        else
            if [ "${2}" -eq 0 ] && [ "$3" -eq 0 ]; then    # Confirm
                # rua 
                yay --needed -S "${1}";
            elif [ "${2}" -eq 0 ] && [ "$3" -eq 1 ]; then  # Confirm and Force
                yay --needed --force -S "${1}";
            elif [ "${2}" -eq 1 ] && [ "$3" -eq 1 ]; then  # No Confirm and Force
                yay --needed --noconfirm --force -S "${1}";
            elif [ "${2}" -eq 1 ] && [ "$3" -eq 0 ]; then  # No Confirm
                yay --needed --noconfirm -S "${1}";
            else
                yay --noconfirm --needed -S "${1}";   # Default No Confirm
            fi
        fi
    elif [ "${My_DIST}" == "debian" ]; then # ------------------------------ Debian: My_PSUEDONAME = Ubuntu, LMDE - Distros
        if [ "${2}" -eq 0 ] && [ "$3" -eq 0 ]; then    # Confirm
            apt-get install "${1}";
        elif [ "${2}" -eq 0 ] && [ "$3" -eq 1 ]; then  # Confirm and Force
            apt-get -f install "${1}";
        elif [ "${2}" -eq 1 ] && [ "$3" -eq 1 ]; then  # No Confirm and Force
            apt-get -f -y install "${1}";
        elif [ "${2}" -eq 1 ] && [ "$3" -eq 0 ]; then  # No Confirm
            apt-get -y install "${1}";
        else
            apt-get -y install "${1}";   # Default No Confirm
        fi
    elif [ "${My_DIST}" == "mandrake" ]; then # ---------------------------- My_PSUEDONAME = Mandrake Distros
        urpmi "${1}"
    elif [ "${My_DIST}" == "unitedlinux" ]; then # ------------------------- My_PSUEDONAME = unitedlinux Distros
        zypper install "${1}";
    elif [ "${My_DIST}" == "suse" ]; then # -------------------------------- My_PSUEDONAME = Suse Distros
        zypper install "${1}";
    elif [ "${My_DIST}" == "slackware" ]; then # -------------------------------- My_PSUEDONAME = Slackware Distros
        installpkg "${1}";
        # isInstalled $1 $2
        # if grep $1 $2; then return 0; else return 1; fi
        # grep /sbin/iptables /var/log/packages/*
    fi
}
###############################################################################
###############################################################################
setOsInfo()
{
    local -i thisPrintDebug;  thisPrintDebug=1;      # 1=true or 0=false
    local -i thisSetDebug;    thisSetDebug=0;        # 1=true or 0=false
    if [ "${thisSetDebug}" -eq 1 ]; then set -x; fi  # Turn On Debug mode
    if [ "${thisPrintDebug}" -eq 1 ]; then
        print_test "Debugging Information for" "$(basename "${BASH_SOURCE[0]}") -> ${FUNCNAME[0]}() : ${LINENO[0]}";
    fi
    My_OS="$(to_lower_case "$(uname -s)")";  # OS: Linux, FreeBSD, MAC, Windows
    My_OS="${My_OS// /}";                    # Remove all Spaces: Windows Nt becomes WindowsNt
    My_ARCH="$(uname -m)";                   # x86 or x86_64
    My_Ver="$(uname -r)";                    # OS Version
    #
    if [ "$My_OS" == "sunos" ]; then           # Sun OS 
        case $(uname -r) in
            4*)  My_OS="sunbsd";   ;;
            5*)  My_OS="solaris";  ;;
             *)  My_OS="solaris";  ;;
        esac
        My_ARCH="$(uname -p)";
        My_DIST="$My_OS";
    elif [ "$My_OS" == "HP-UX" ]; then         # HP-UX
        My_OS="hp";
        My_DIST="hp";
    elif [ "$My_OS" == "IRIX" ]; then          # IRIX
        My_OS="sgi";
        My_DIST="sgi";
    elif [ "$My_OS" == "OSF1" ]; then          # OSF1
        My_OS="decosf";
        My_DIST="decosf";
    elif [ "$My_OS" == "ULTRIX" ]; then        # ULTRIX
        My_OS="ultrix";
        My_DIST="ultrix";
    elif [ "$My_OS" == "aix" ]; then           # AIX
        My_Ver=$(oslevel -r);
        My_DIST="aix";
    elif [ "$My_OS" == "freebsd" ]; then       # Free BSD
        My_DIST="freebsd";
    elif [ "$My_OS" == "windowsnt" ]; then     # Windows NT
        My_DIST="windowsnt";
    elif [ "$My_OS" == "darwin" ]; then        # MAC
        My_OS="mac";
        My_DIST="mac";
        # shellcheck disable=SC2230
        if [ -n "$(which sw_vers 2>/dev/null)" ]; then
            My_PSUEDONAME="$(sw_vers -productName)";
            My_Ver="$(sw_vers -productVersion)";
            My_OS_Update="${My_Ver##*.}";
            My_Ver="${My_Ver%.*}";
            if [[ "$My_Ver" =~ 10.8 ]]; then
                My_PSUEDONAME="mountain lion";
            elif [[ "$My_Ver" =~ 10.7 ]]; then
                My_PSUEDONAME="lion";
            elif [[ "$My_Ver" =~ 10.6 ]]; then
                My_PSUEDONAME="snow leopard"
            elif [[ "$My_Ver" =~ 10.5 ]]; then
                My_PSUEDONAME="leopard";
            elif [[ "$My_Ver" =~ 10.4 ]]; then
                My_PSUEDONAME="tiger";
            elif [[ "$My_Ver" =~ 10.3 ]]; then
                My_PSUEDONAME="panther";
            elif [[ "$My_Ver" =~ 10.2 ]]; then
                My_PSUEDONAME="jaguar";
            elif [[ "$My_Ver" =~ 10.1 ]]; then
                My_PSUEDONAME="puma";
            elif [[ "$My_Ver" =~ 10.0 ]]; then
                My_PSUEDONAME="cheetah";
            else
                My_PSUEDONAME="unknown";
            fi
        fi
    elif [ "$My_OS" == "linux" ]; then                                                                 # Linux
        # shellcheck disable=SC1123
        if [ -f /etc/centos-release ]; then                                                            # CentOS
            My_DIST="redhat";
            My_PSUEDONAME="CentOS";
            My_Ver_Major="$(cut -d " " -f 3 /etc/redhat-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/redhat-release | cut -d '.' -f 2)";
            My_Ver="${My_Ver_Major}.${My_Ver_Minor}"; # $(cat /etc/redhat-release | sed s/.*release\ // | sed s/\ .*//);
            echo "My_Ver=$My_Ver";
            My_OS_Update="$(uname -r)";
        elif [ -f /etc/redhat-release ]; then                                                          # Redhat
            My_DIST="redhat";
            My_PSUEDONAME="Redhat";
            My_Ver_Major="$(cut -d " " -f 3 /etc/redhat-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/redhat-release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/redhat-release | sed s/.*release\ // | sed s/\ .*//)";
        elif [ -f /etc/fedora-release ]; then                                                          # Fedora
            My_DIST="fedora";
            My_PSUEDONAME="Fedora";
            My_Ver_Major="$(cut -d " " -f 3 /etc/fedora-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/fedora-release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/fedora-release | sed s/.*release\ // | sed s/\ .*//)";
        elif [ -f /etc/SuSE-release ]; then                                                            # SuSE
            My_DIST="suse";
            My_PSUEDONAME="SuSe";
            My_Ver_Major="$(cut -d " " -f 3 /etc/SuSE-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/SuSE-release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/SuSE-release | tr "\n" " " | sed s/.*=\ //)";
        elif [ -f /etc/mandrake-release ]; then                                                        # Mandrake
            My_DIST="mandrake";
            My_PSUEDONAME="Mandrake";
            My_Ver_Major=$(cut -d " " -f 3 /etc/mandrake-release | cut -d '.' -f 1);
            My_Ver_Minor=$(cut -d " " -f 3 /etc/mandrake-release | cut -d '.' -f 2);
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/mandrake-release | sed s/.*release\ // | sed s/\ .*//)";
        elif [ -f /etc/yellowdog-release ]; then                                                       # Yellow dog
            My_DIST="yellowdog";
            My_PSUEDONAME="YellowDog";
            My_Ver_Major="$(cut -d " " -f 3 /etc/yellowdog-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/yellowdog-release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/yellowdog-release | tr "\n" " " | sed s/.*=\ //)";
        elif [ -f /etc/sun-release ]; then                                                             # Sun JDS
            My_DIST="sun";
            My_PSUEDONAME="Sun";
            My_Ver_Major="$(cut -d " " -f 3 /etc/sun-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/sun-release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/sun-release | tr "\n" " " | sed s/.*=\ //)";
        elif [ -f /etc/release ]; then                                                                 # Solaris/Sparc
            My_DIST="solaris";
            My_PSUEDONAME="Sparc";
            My_Ver_Major="$(cut -d " " -f 3 /etc/release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/release | tr "\n" " " | sed s/.*=\ //)";
        elif [ -f /etc/gentoo-release ]; then                                                          # Gentoo
            My_DIST="gentoo";
            My_PSUEDONAME="Gentoo";
            My_Ver_Major="$(cut -d " " -f 3 /etc/gentoo-release | cut -d '.' -f 1)";
            My_Ver_Minor="$(cut -d " " -f 3 /etc/gentoo-release | cut -d '.' -f 2)";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/gentoo-release | tr "\n" " " | sed s/.*=\ //)";
        elif [ -f /etc/debian_version ]; then    # Old Debian--->/etc/debian_release or /etc/debian_version New Debian--->/etc/os-release  used by Debian, Ubuntu, LMDE
            My_DIST="debian";
            if grep -Fxq "Debian" /etc/issue; then
                My_PSUEDONAME="Debian";
            elif grep -Fxq "Ubuntu" /etc/issue; then
                My_PSUEDONAME="Ubuntu";
            elif grep -Fxq "Linux Mint Debian Edition" /etc/issue; then
                My_PSUEDONAME="LMDE";
            elif grep -Fq "LMDE" /etc/issue; then
                My_PSUEDONAME="LMDE";
            else
                My_PSUEDONAME="Debian";
            fi
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/lsb-release | grep '^DISTRIB_RELEASE' | awk -F=  '{print $2}')";
        elif [ -f /etc/arch-release ]; then                                                            # ArchLinux They may drop this like Manjaro did
            My_DIST="archlinux";
            My_PSUEDONAME="Archlinux";
            My_OS_Package="pacman";
            My_Ver="$( lsb_release -r -s )";                     # LMDE=1
            if [[ "${My_Ver}" == *.* ]]; then # 5=Anywhere
                My_Ver_Major="$(string_split "$My_Ver" "." 1)";  # 1=First Half
                My_Ver_Minor="$(string_split "$My_Ver" "." 2)";  # 2=Second Half
                My_OS_Update="${My_Ver##*.}";
            else
                My_Ver_Major="$My_Ver";
                My_Ver_Minor="0";
                My_OS_Update="0";
            fi
        elif [ -f /etc/os-release ]; then                                                              # Manjaro - ArchLinux
            My_DIST="archlinux";
            My_PSUEDONAME="Manjaro";
            My_OS_Package="pacman";
            My_Ver="$( lsb_release -r -s )";                     # LMDE=1
            if is_needle_in_haystack "." "$My_Ver" 5; then # 5=Anywhere
                My_Ver_Major="$(string_split "$My_Ver" "." 1)";  # 1=First Half
                My_Ver_Minor="$(string_split "$My_Ver" "." 2)";  # 2=Second Half
            else
                My_Ver_Major="$My_Ver";
                My_Ver_Minor="0";
            fi
        elif [ -f /etc/UnitedLinux-release ]; then                                                     # United Linux
            My_DIST="unitedlinux";
            My_PSUEDONAME="UnitedLinux";
            # shellcheck disable=SC2002
            My_Ver="$(cat /etc/UnitedLinux-release | tr "\n" " " | sed s/VERSION.*//)";
        elif [ -f /etc/slackware-release ] || [ -f /etc/slackware-version ]; then                      # slackware
            My_DIST="slackware";
            My_PSUEDONAME="slackware";
            if [ -f /etc/slackware-release ]; then
                # shellcheck disable=SC2002
                My_Ver="$(cat /etc/slackware-release | tr "\n" " " | sed s/VERSION.*//)";
            else
                # shellcheck disable=SC2002
                My_Ver="$(cat /etc/slackware-version | tr "\n" " " | sed s/VERSION.*//)";
            fi            
        elif [[ -x "$(type -p lsb_release 2>/dev/null)" ]]; then # shellcheck disable=SC2230
            # which replaced with type
            # Debian
            # /usr/bin/lsb_release
            #My_PSUEDONAME=$(  lsb_release -i -s);             # LinuxMint
            My_Ver="$( lsb_release -r -s )";                     # LMDE=1
            if is_needle_in_haystack "." "$My_Ver" 5; then # 5=Anywhere
                My_Ver_Major="$(string_split "$My_Ver" "." 1)";  # 1=First Half
                My_Ver_Minor="$(string_split "$My_Ver" "." 2)";  # 2=Second Half
            else
                My_Ver_Major="$My_Ver";
                My_Ver_Minor="0";
            fi
            My_DIST="$(lsb_release -c -s)";                      # debian
            My_OS_Package="rpm";
            if [[ "$My_PSUEDONAME" =~ Debian,Ubuntu ]]; then
                My_OS_Package="deb";
            elif [[ "SUSE LINUX" =~ $My_PSUEDONAME ]]; then
                if lsb_release -d -s | grep -q openSUSE; then
                    My_PSUEDONAME="openSUSE";
                fi
            elif [[ $My_PSUEDONAME =~ Red.*Hat ]]; then
                My_PSUEDONAME="Red Hat";
            fi
            if grep -Fxq "Debian" /etc/issue; then
                My_PSUEDONAME="Debian";
                My_OS_Package="deb";
            elif grep -Fxq "Ubuntu" /etc/issue; then
                My_PSUEDONAME="Ubuntu";
                My_OS_Package="deb";
            elif grep -Fxq "Linux Mint Debian Edition" /etc/issue; then
                My_PSUEDONAME="LMDE";
                My_OS_Package="deb";
            elif grep -Fq "LMDE" /etc/issue; then
                My_PSUEDONAME="LMDE";
                My_OS_Package="deb";
            else
                My_PSUEDONAME="Debian";
                My_OS_Package="deb";
            fi
        fi
    fi
    #
    My_Dev_Folder="${HOME}\Dev";
}
################################ End setOsInfo ################################
###############################################################################
doUpdate()
{
    if [ "$My_OS" == "solaris" ]; then
        pkg update;
    elif [ "$My_OS" == "aix" ]; then
        smit update;
    elif [ "$My_OS" == "freebsd" ]; then
        pkg update;
    elif [ "$My_OS" == "windowsnt" ]; then
        # Running Windows Server, not sure if you have haproxy monit fail2ban;
        # I will have to test WeBook to see how to handle it
    elif [ "$My_OS" == "mac" ]; then
        brew update;
    elif [ "$My_OS" == "linux" ]; then
        # if there is a need to know which Distro; i.e. CentOS for Redhat, or Ubuntu for Debian, then use My_PSUEDONAME
        # See wizard.sh setOsInfo to see if OS test exist or works
        if [ "${My_DIST}" == "redhat" ]; then # -------------------------------- Redhat, Centos, Fedora
            yum-complete-transaction && package-cleanup --dupes && package-cleanup --dupes && package-cleanup --problems && yum update -y && yum upgrade -y;
        elif [ "${My_DIST}" == "archlinux" ]; then # --------------------------- My_PSUEDONAME = Archlinux Distros
            pacman -Syu --noconfirm;
        elif [ "${My_DIST}" == "debian" ]; then # ------------------------------ Debian: My_PSUEDONAME = Ubuntu, LMDE - Distros
            apt-get update -y && apt-get upgrade -y && apt-get dist-upgrade -y && dpkg --configure -a;
            if [ "$My_PSUEDONAME" == "Ubuntu" ]; then
                echo "Ubuntu";
            else
                echo "debian";
            fi
        elif [ "${My_DIST}" == "mandrake" ]; then # ---------------------------- My_PSUEDONAME = Mandrake Distros
            urpmi --auto-update;
        elif [ "${My_DIST}" == "unitedlinux" ]; then # ------------------------- My_PSUEDONAME = unitedlinux Distros
            zypper update;
        elif [ "${My_DIST}" == "suse" ]; then # -------------------------------- My_PSUEDONAME = Suse Distros
            zypper update;
        elif [ "${My_DIST}" == "slackware" ]; then # --------------------------- My_PSUEDONAME = Slackware Linux Distros
            upgradepkg;
        fi
    fi
} 
################################ END doUpdate #################################
###############################################################################
doInstall()
{
    # Make sure that a real Password and Salt is used
    if [ "$MySupperSecretPassword" == "NO-PASSWORD" ] || [ "$MySalt" == "NO-SALT" ]; then
        showUsage;
    fi
    #
    testPasswordSafe;
    if [ "$MyLastCommandPassed" -eq 0 ]; then
        local -a thisReturnStatus; 
        thisReturnStatus="$(passwordSafe "${MySupperSecretPassword}" "${MySalt}" "encrypt" )";
        # Store in ini file if needd
        thisReturnStatus="$(passwordSafe "$thisReturnStatus" "${MySalt}" "decrypt" )";
        # We only need it to create the User Account
        # There is a reason for this feature, its just not finished yet, 
        # but mainly so we can pass in an Encrypted password, instead of doing it this way
        # FIXME Finish this feature
    fi
    #
    # Start doing an OS specific Install
    #
    if [ "$My_OS" == "solaris" ]; then
        pkg install haproxy monit fail2ban;
        #
        # Create user webook
        #
        if ! is_user webook; then
            groupadd webook;
            useradd -m -g webook -G webook,users -s /bin/bash webook;
            echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
            mkdir -p /home/webook/run;
            mkdir -p /home/webook/build;
            chown -R webook:webook /home/webook;
            #nano /etc/sudoers
            # add
            echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
        fi
    elif [ "$My_OS" == "aix" ]; then
        smit install haproxy monit fail2ban;
        #
        # Create user webook
        #
        if ! is_user webook; then
            groupadd webook;
            useradd -m -g webook -G webook,users -s /bin/bash webook;
            echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
            mkdir -p /home/webook/run;
            mkdir -p /home/webook/build;
            chown -R webook:webook /home/webook;
            #nano /etc/sudoers
            # add
            echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
        fi
    elif [ "$My_OS" == "freebsd" ]; then
        pkg install haproxy monit fail2ban;
        #
        # Create user webook
        #
        if ! is_user webook; then
            groupadd webook;
            useradd -m -g webook -G webook,users -s /bin/bash webook;
            echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
            mkdir -p /home/webook/run;
            mkdir -p /home/webook/build;
            chown -R webook:webook /home/webook;
            #nano /etc/sudoers
            # add
            echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
        fi
    elif [ "$My_OS" == "windowsnt" ]; then
        # Running Windows Server, not sure if you have haproxy monit fail2ban;
        # I will have to test WeBook to see how to handle it
        #
        # Create user webook
        #
        if ! is_user webook; then
            groupadd webook;
            useradd -m -g webook -G webook,users -s /bin/bash webook;
            echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
            mkdir -p /home/webook/run;
            mkdir -p /home/webook/build;
            chown -R webook:webook /home/webook;
            #nano /etc/sudoers
            # add
            echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
        fi
    elif [ "$My_OS" == "mac" ]; then
        brew install haproxy monit fail2ban;
        #
        # Create user webook
        #
        if ! is_user webook; then
            groupadd webook;
            useradd -m -g webook -G webook,users -s /bin/bash webook;
            echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
            mkdir -p /home/webook/run;
            mkdir -p /home/webook/build;
            chown -R webook:webook /home/webook;
            #nano /etc/sudoers
            # add
            echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
        fi
    elif [ "$My_OS" == "linux" ]; then
        # if there is a need to know which Distro; i.e. CentOS for Redhat, or Ubuntu for Debian, then use My_PSUEDONAME
        # See wizard.sh setOsInfo to see if OS test exist or works
        if [ "${My_DIST}" == "redhat" ]; then # -------------------------------- Redhat, Centos, Fedora
            yum install yum-complete-transaction;
            yum-complete-transaction && package-cleanup --dupes && package-cleanup --dupes && package-cleanup --problems && yum update -y && yum upgrade -y;
            yum install haproxy monit fail2ban sshpass rsync openssh-server openssh-client -y;
            yum install gcc gcc-c++ autoconf automake cmake openssl pango pango-devel libtiff zlib glibc nano gd gd-devel rsync yum-utils bind-utils -y;
            yum groupinstall install "Development Tools" -y;
            yum erase -y php-common; 
            
            service haproxy start; chkconfig haproxy on;
            service monit start; chkconfig --levels 235 monit on;
            # yum groupinstall install x -y
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
        elif [ "${My_DIST}" == "archlinux" ]; then # --------------------------- My_PSUEDONAME = Archlinux Distros
            pacman-mirrors --country United_States;
            pacman-mirrors --fasttrack 6;
            #pacman -Syu --noconfirm;
            pacman -Syyu --noconfirm; # This forces a full update, it can break things, so take care
            
            pacman --noconfirm --needed -S haproxy monit fail2ban base-devel wget rsync nano gcc autoconf automake cmake clang net-tools inetutils lshw;
            pacman --noconfirm --needed -S qt5-base qt5-networkauth qt5-tools qt5-translations qt5-websockets qt5-imageformats qt5-webkit;
            pacman --noconfirm --needed -S gdb git tk valgrind doxygen tree clamav imagemagick openjpeg2 sshpass rsync openssh openssl pango libtiff zlib glibc gd;
            
            pacman --noconfirm --needed -S xorg-font-util xorg-fonts-75dpi xorg-fonts-encodings xorg-fonts-misc xorg-fonts-misc xorg-fonts-type1 xorg-fonts-cyrillic xorg-fonts-alias;
            pacman --noconfirm --needed -S fontconfig xorg-xlsfonts awesome-terminal-fonts;
            pacman --noconfirm --needed -S ttf-liberation ttf-ubuntu-font-family noto-fonts ttf-roboto ttf-droid ttf-dejavu ttf-croscore ttf-bitstream-vera font-bh-ttf;
            pacman --noconfirm --needed -S ttf-anonymous-pro ttf-cascadia-code ttf-fira-mono gnu-free-fonts ttf-linux-libertine;
            pacman --noconfirm --needed -S adobe-source-code-pro-fonts adobe-source-sans-pro-fonts;
            pacman --noconfirm --needed -S xorg-fonts-type1 font-mathematica;
            pacman --noconfirm --needed -S dina-font tamsyn-font terminus-font bdf-unifont terminus-font-otb ttf-fantasque-sans-mono;
            
            freshclam;
            systemctl start clamav-daemon.service;
            systemctl status clamav-daemon.service;
            systemctl enable clamav-daemon.service;
            systemctl start haproxy; systemctl enable haproxy;
            systemctl monit start; systemctl enable monit;
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
        elif [ "${My_DIST}" == "debian" ]; then # ------------------------------ Debian: My_PSUEDONAME = Ubuntu, LMDE - Distros
            apt-get update -y && apt-get upgrade -y && apt-get dist-upgrade -y && dpkg --configure -a;
            apt-get install haproxy monit fail2ban -y;
            apt-get install build-essential aptitude gtkorphan rsync cmake gcc g++ doxygen automake libtool flex bison pkg-config g++ libssl-dev -y;
            apt-get install graphicsmagick libgraphicsmagick1-dev libgraphicsmagick3 -y;
            apt-get install gcc cmake doxygen openssh-server openssh-client sshpass rsync openssh-server openssh-client dnsutils -y;
            service haproxy start; chkconfig haproxy on;
            service monit start; chkconfig --levels 235 monit on;
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
            if [ "$My_PSUEDONAME" == "Ubuntu" ]; then
                echo "Ubuntu";
            else
                echo "debian";
            fi
        elif [ "${My_DIST}" == "mandrake" ]; then # ---------------------------- My_PSUEDONAME = Mandrake Distros
            urpmi haproxy monit fail2ban;
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
        elif [ "${My_DIST}" == "unitedlinux" ]; then # ------------------------- My_PSUEDONAME = unitedlinux Distros
            zypper install haproxy monit fail2ban;
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
        elif [ "${My_DIST}" == "suse" ]; then # -------------------------------- My_PSUEDONAME = Suse Distros
            zypper install haproxy monit fail2ban;
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
        elif [ "${My_DIST}" == "slackware" ]; then # -------------------------------- My_PSUEDONAME = Slackware Linux Distros
            #upgradepkg --install-new
            installpkg haproxy monit fail2ban;
            #
            # Create user webook
            #
            if ! is_user webook; then
                groupadd webook;
                useradd -m -g webook -G webook,users -s /bin/bash webook;
                echo -e "${MySupperSecretPassword}\n${MySupperSecretPassword}\n" | passwd webook;
                mkdir -p /home/webook/run;
                mkdir -p /home/webook/build;
                chown -R webook:webook /home/webook;
                #nano /etc/sudoers
                # add
                echo "webook ALL=(ALL) ALL" >> /etc/sudoers;
            fi
        fi
    fi
    
    #if [ "$MyWeBookServerFullPathExecutable" != "" ]; then fi
    
    
    if [ "${MyBuildPath}" != "" ]; then
        if cd "${MyBuildPath}"; then
            git clone --recursive --recurse-submodules "$MyQHttpServerRepo";
        else
            echo "Error cd to build folder : ${MyBuildPath}";
            exit 1;
        fi
    fi
    #
    if [ -d "${MyBuildPath}/${MyDefaultWeBookFolder}" ]; then
        MyWeBookFolderInstallerIntoExamples = "${MyBuildPath}/${MyDefaultWeBookFolder}"
    else
        mkdir -p "${${MyBuildPath}/${MyDefaultWeBookFolder}}";
        MyWeBookFolderInstallerIntoExamples = "${MyBuildPath}/${MyDefaultWeBookFolder}"
    fi
    #
    if cd "$MyWeBookFolderInstallerIntoExamples"; then
        git clone "$MyWeBookServerRepo";
    fi
    
    
} 
################################ END doInstall ################################
###############################################################################
doBuild()
{
    if [ "${MyBuildPath}" != "" ]; then
        if cd "${MyBuildPath}"; then
            git pull "$MyQHttpServerRepo";
        else
            echo "doBuild Error cd to build folder : ${MyBuildPath}";
            exit 1;
        fi
    fi
    #
    if [ -d "${MyBuildPath}/${MyDefaultWeBookFolder}" ]; then
        MyWeBookFolderInstallerIntoExamples = "${MyBuildPath}/${MyDefaultWeBookFolder}"
    else
        # this is an error, FIXME
        mkdir -p "${${MyBuildPath}/${MyDefaultWeBookFolder}}";
        MyWeBookFolderInstallerIntoExamples = "${MyBuildPath}/${MyDefaultWeBookFolder}"
    fi
    #
    if cd "$MyWeBookFolderInstallerIntoExamples"; then
        git pull "$MyWeBookServerRepo";
    else
        echo "doBuild Error cd to build folder : ${MyWeBookFolderInstallerIntoExamples}";
        exit 1;
    fi
    #
    if cd "${MyBuildPath}"; then
        # FIXME git upstream
        find "${MyBuildPath}" -type f -name '*.pro.user' -delete;
        find "${MyBuildPath}" -type f -name '*.pro.*.user' -delete;
        qmake qthttpserver.pro;
        make clean;
        make;
        # /home/webook/build/WeBook/examples/httpserver/webookserver
        if cd "${MyBuildPath}/${MyDefaultWeBookFolder}"; then
            qmake webookserver.pro;
            make clean;
            make;
            if [ -f "${MyBuildPath}/${MyDefaultWeBookFolder}/webookserver" ]; then
                MyWeBookServerFullPathExecutable="${MyBuildPath}/${MyDefaultWeBookFolder}/webookserver";
                export MyWeBookServerFullPathExecutable;    
            else
                echo "Error Compiled file not found: ${MyBuildPath}/${MyDefaultWeBookFolder}/webookserver";
            fi
        else
            echo "Error cd Folder: ${MyBuildPath}/${MyDefaultWeBookFolder}";
        fi    
    
    else
        echo "Error cd Folder: $MyBuildPath";
    fi
}
################################ End doBuild ##################################
#
setOsInfo;
# Install does doUpdate first
if [ "${MySwitch}" == "Install" ]; then
    doUpdate;
    setPaths;
    doInstall;
fi
# Update is the only option that only updates
if [ "${MySwitch}" == "Update" ]; then
    doUpdate;
fi
# Build does Update First
if [ "${MySwitch}" == "Build" ]; then
    doUpdate;
    setPaths;
    doBuild;
fi
###############################################################################

################################ End of File ##################################
