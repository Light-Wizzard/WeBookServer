# WeBookServer is Pronounced Web Book Server

This project is written in Qt C++, 
it has a Client called WeBookClient,
that is also written in Qt C++,
the WeBook Project itself is an example of how to use the two projects together.

This project is currently in Alpha,
the reason I have posted it, is to test the installation script, 
and it will check GitHub for updates before continuing,
so I needed to post even a skeleton of the code in order to even develop it.

Currently there is not much to look at, it creates an HTTP Server using QHttpServer,
and looks for the WeBookClient Data Folders,
where it will load and display its content.


## Status
Unix: [![Travis Build Status](https://travis-ci.org/Light-Wizzard/WeBookServer.svg?branch=master)](https://travis-ci.org/Light-Wizzard/WeBookServer)

## :package: **Downloads:**
 - :penguin: [Linux](https://github.com/Light-Wizzard/WeBookServer/releases/download/continuous/WeBookServer-travis-x86_64.AppImage)
 - :penguin: :inbox_tray: [Linux ZSync](https://github.com/Light-Wizzard/WeBookServer/releases/download/continuous/WeBookServer-travis-x86_64.AppImage.zsync)


## Work Flow

Currently I am using GitHub projects to do the heavy work like Web Server,
and to make it run as a Daemon in Unix and a Windows Service in Windows,
I am using QtHttpServer, and QtService by QtLabs for those jobs.

QtHttpServer:
https://github.com/qt-labs/qthttpserver

QtService:
https://github.com/qtproject/qt-solutions/tree/master/qtservice

I decided to compile it, rather then joining it as a subdir project for one reason,
they are in development, and can break my code any day,
so I must first test all there updates before pushing them in my project,
and I will keep it this way, all code is compiled in.
The main reason for this is simple, this app is small,
so no real need for a Library.

I wrote the program to take switches, 
and to make up defaults if those switches are blank,
so it has a way of reading in configuration files,
but with so many Operating Systems to deal with,
I decided a Database was out, 
too many to have to deal with,
and why add that complexity to a much simpler problem,
and then I had Qt Settings,
nice, but I have to deal with a few issues,
and one drove me crazy just thinking about, 
let me explain,
to use Qt Setting, which is nice,
you can use the Default settings,
it store it with that applications data,
and support is great, 
but then I got to wondering, 
what if I want to set the Organization, 
I have no idea what it is ahead of time,
unless you pass it in as a command line switch,
and you can,
and it is set up to use Qt Settings by default this way,
now here is my issue, 
one first run, Qt Settings will always, 
and that word ALWAYS is like the word ONLY,
as in ONLY Congress can Print Money,
so to Americans I have to explain this in Detail,
in all cases, ONLY and ALWAYS,
can never be changed or Amended, 
meaning Minor Change,
you void the contract with them when you alter their meaning,
so I cannot rely on them,
not at start up,
so I need to write a function that reads in a file that exists or the app dies,
and I got to thinking if I write a function that has to read and write this information anyway,
why use Qt Settings,
but still, its there, and still useful after the second run.

This app will try to minimize the amount of switches,
it takes 1 arguments: Install, Update, Build, or Run.

Install will try to figure out which OS you are running, and what Distribution of Linux or Unix,
I get this information from QSysInfo Class https://doc.qt.io/qt-5/qsysinfo.html,
WeBookServer should run on a Windows Server, but not sure about setting it up as a Service,
but eventually I will work on that, and get it to work, 
not sure QtHttpServer supports IIS, so this may not be a Windows thing,
but I do try to support as many distribution of Unix, so to name a few,
Linux, Arch Linux or Manjaro, FreeBSD, MAC, Sun JDS, Solaris/Sparc, Sunbsd, HP-UX, IRIX, OSF1, ULTRIX, AIX,
CentOS, Debian, Redhat, Slackware, Suse, Unitedlinux, Mandrake, Yellow Dog, Gentoo,
and actually this list is just stuff I know it works on, this is Qt and C++,
so it should run on anything, making this list meaningless,
other than to make you understand what I have to deal with.

I was going to write this as a bash script, but decided not to for many reasons,
in fact, I decided to write it as a Service, and that takes a bit more work,
and I also want it to work with Description [HAProxy](https://github.com/haproxy/haproxy) and [Monit](https://github.com/arnaudsj/monit),
more on that later,
but I realized that the bash scripts were limiting me to the above,
and not Qt, so why do that to myself or others.

I have only tested this on Arch Linux as my Server and Manjaro as my Desktop,
so if you have any issues file them, and be specific, and I will try to resolve them.

This app is meant to be run on a VPS or Server,
and it should work on Windows, as well as being written for Unix,
meaning Linux or Mac, but really any Machine that Qt Supports,
this is a Web Server, much like Apache or NGINX,
and I think you can run those on Windows, so no reason you cannot run WeBookServer on it,
but IIS is another issue, but there are always ways to deal with it.

You need to down the installer from WeBookServer GitHub,
https://github.com/Light-Wizzard/WeBookServer,
now that said,
this is Alpha,
so, do not judge it by what is there now, 
but what it will become,
and I have most of the code already written from 6 years ago,
and I have updated most of,
but now its a matter of integrating it all into one app,
and I decided not to make this with Plugins for many reasons,
security being the show stopper.

Having decided not to write bash scripts,
no easy for me,
but I decided to write an installer instead,
then decided, instead I will just write the WeBookClient,
to install the WeBookServer,
and so it will.
Now once you click on "Install Server",
a Wizard will have you fill out a form,
with the required fields required for Authentication,
User Name, and Password,
and I got to thinking about how to save this information,
and decided not to,
in fact, I decided not to use any type of Authentication currently in use,
and most people will think that is crazy,
but I say what works, works.

## Installer

Now the Installer will download QtHttpServer from GitHub,
directly from the Server you log into,
and it must have the ability to create a new user,
or have an existing one setup for it,
you log in as root, otherwise you do not have that option,
and wanting this to run on any type of account,
I have a Wizard to deal with all types of Accounts,
so it will ask you what type of account it is,
and if Admin is an Option,
then you have more options to deal with,
I wrote WeBookServer to be a Server Controller,
so it will only run to its fullest potential as root,
as a user, it has limited ability,
but still, we make the best of what we have available to us,
and we also have Local Mode, 
this does not require any Server,
just a folder,
so you need two folders,
and you may wonder why two,
and the answer is simple, 
WeBookClient has a Rich Text Editor,
that translates to HTML code,
so you have a document you are working on,
and its like a draft,
so this is a draft folder,
and when I hit Publish,
I want it to go to another folder,
now this can be a Local folder,
a Mount, an NAS, the Internet is not Local,
but still an option, this can be a VPS, or even another Machine you own,
and its on a Network connection,
and it can be any OS,
the concept is simple, you find a place to install WeBookServer,
and WeBookClient,
the Server only sees what is published,
now think about this,
the way I develop this in Linux,
is I pull up a Terminal, 
and change directory to the folder where the Qt Compiler put the executable,
and I run it,
but I am on Linux,
and any Unix machine works the same way, even Mac,
and even on Windows, I can run the Server, and Client,
but keep in mind, on Windows, 
this will only run under Description [PowerShell](https://docs.microsoft.com/en-us/powershell/scripting/overview?view=powershell-7) with Unix for Windows installed,
and [Cygwin](https://www.cygwin.com/), more on this later,

## Version Control and Alphabet Soup Decoded

The Thing about Alpha is this, what the software does now is called a Starting point, 
and what it will do when finished is called beyond Able, Affirmative, Afirm, Alfa, just to get to Alpha, 
then you have Bravo, Boy, Baker just to get to Beta, and Charlie we can Cast,
and Delta is always a Dog and Echo is Easy, but boring after a while, and Foxtrot is a Fox,
but what can I say George loves to Golf,
and How the Hotel Hypo Have got you as far as India,
where an India of Interrogatory became an Int,
then Juliett did the Jig for Kilo King,
that Lima Love, 
as does Mike in November, 
but it was a Negative, or Negat, or even Nan,
on the Oscar, Oboe was not an Option,
ask Papa about the Preparatory Pup Prep from Quebec,
made by some Queen of Quack in a Rush to meet Roger just to be Romeo,
and Sierra was made of Sugar Sail, 
and Tango got Tare,
by the Uncle Uniform Unit, who had the Victor Vice while drinking Whiskey, 
while he Watch William X-ray take by some Yoke Yankee, 
named Zed, at Zulu with a Zebra,
all of these Alphabet Soup Letter Call Signs,
just to get to version 1, so call this 0.0.1, Major.Minor.Patch,
so I hope that is clear,
if not, the difference between Alpha, 
and Beta are two different stories,
and it all depends on your level of commitment,
as to if you want to try this now,
because this is not near Beta yet,
just Alpha, and a long way from Zulu,
but if you are a Programming,
and want to learn about QtHttpServer Server-Client programming,
then this is the place to be,
and the time to be there.

## Server Installed

Once you have the Server installed, it will look for a WeBook.cat file,
that is created by WeBookClient,
by default if has one entry, WeBook,
and that represents a Book, so if it does not find one, it makes it.

You can have unlimited amount of WeBooks you can add, each represent data differently,
for example,
you can have one WeBook per URL,
or have each WeBook be a Tree,
for example,
say I have WeBook as my first WeBook,
and Videos as my Second,
and I might add Music down the line, so I need to decide what layout I want,
by URL or by Tree.

A Tree is a Hierarchal view, 
each WeBook has a Table of Content AKA TOC,
it is also a Tree View,
the Top element is the Title Page,
it has an H1 Tag in the HTML Document,
and you can only have on H1 Tag per Document,
all Chapters will have H2 Tags,
all Sub Chapters will have H3 Tags,
and it only goes 6 levels deep,
because H6 is the last H Tag.

WeBook knows your Layout because of the Files it generates,
these are text files that are indention based,
WeBook does not use any Databases, the reason is Simple,
it does not need them for Speed,
it does not use a look-ups to find data,
the data is routed one for one in the code,
and this is due to the Tree view,
so let me explain how it works.

If you look at any WeBook.cat file, you will notice they have this pattern:

```
RootID  Root Title
  ChildID Child Title
    ChildrenID Children Title
```

RootID is a Column in the Treeview, it has a Tab (4 spaces \t character) as a delimiter,
and each row is a child of what is above it,
each indent, not Tab, is 2 spaces, 
you must save this document with Tabs on,
a Tab is actually the character \t,
\n is a New line,
and they are at the end of all lines, and \r,
which is a Windows thing,
most be stripped,
this is done automatically, 
because I have a Windows Machine also,
and I want this to be Cross-Platform,
so its not Unix Specifics, and I will make every effort to ensure it works on Windows,
Mac not an issue, its Unix,
and so far, IIS is the only issue I am facing,
and its a none player,
but this is the issue.

## WeBookServer Configuration

WeBookServer can be configured to listen on specific ports,
so as long as your Firewall does not block them,
it should work,
but IIS likes to be top dog when it comes to being the file or web server,
and does not always play nice, so I have a lot of testing to do,
and a little hacking, but I know it will work.
WeBookServer is the Web Server, and not Apache, NGINX, or IIS,
and the Install option will uninstall them,
it will ask before uninstalling, so do not worry,
but if you do not want the warning, when you run it, set the switch -s or --safe,
this is called Safe Mode, and it will not uninstall Apache, NGINX, or IIS,
but is not the Default, so be warned, as if the warning dialog will not be enough.

Do not try to mix your current Web Site with WeBook,
and why it uninstalls other Servers by default,
I will explain how to migrate, but there is no plans for any migration scripts,
and once you understand why, you will say to yourself, I do not need them.

## WeBookServer is a Server

WeBookServer is the Server, and has nothing to do with Apache, NGINX, or IIS, 
nor does it want any interaction with them,
they are a Security risk to is,
and it has enough to deal with,
each has its own issues when it comes to dealing with WeBookServer,
Apache will sometimes add headers to all incoming messages,
even if they are not for it, 
an easy fix, and nothing to actually deal with,
safe mode deals with it,
but without it enabled,
things can get strange,
and I have not even implemented it yet,
but this is an app I actually wrote 6 years ago,
and I am only updating it to be a Client-Server,
now NGINX is easy to deal with,
but it still uses up a lot of threads, just like the others,
and IIS, do not make me explain this issue to you,
I will be like the General on South-Park,
[Get Bill Gates in here...](https://youtu.be/EPfsr8BBdA8).

Why run two Web Servers is a question you must ask yourself.

The Concept of WeBookServer by default is to uninstall Apache, NGINX, or IIS,
because of one reason only,
WeBookServer is the Top Dog. 
It can only be fast if it has nothing to compete with,
meaning Listening time, it does not need to share that with another Server,
and if you cannot afford another VPS account, 
just run it in safe mode, I do,
I am running NGINX, but Apache is no problem, just a few issues to deal with,
and they both have issues I can deal with,
but they all pose a Security risk to the way WeBookServer works,
more on that later.

## Install

Install will run the installer commands, and it can detect the OS it is running on,
and it is all done in Qt in C++, this has switches to control its behaviour.
This will install software on the server, and Update will update it,
it will download WeBookServer from GitHub,
and keep it up to date,
so you download the Installer from WeBook,
or any Fork,
and it uses that account to authenticate from,
so you cannot run a Client from one Fork on a Server from another project,
nor another build from this project, or any fork,
its designed to not run from any executable but the one complied from Travis or AppVeyor,
so they will not communicate by design,
this is to prevent hacking, malware, or viruses,
but beware, a Fork can put code into it that is not in this project,
so do not file issues with Forks here,
you have to trust the Fork.

## Update

Update with do an OS update, nothing to do with the app itself,
this is a feature you can automate, keep in mind this is a Server,
and normally you are not standing in front of it,
so it is written to take full control over the server, 
so if you want to install, or uninstall software, 
you can go through the WeBookClient, to manage the Server,
it can talk directly to the server,
and give it commands to run.

## Build

Build is used to update the app, it will run Update before it runs build,
to ensure the system is up to date,
the Server is normally only interacted with through WeBookClient,
it has a built in zSync2 Auto Update built into the installer from GitHub,
and it has a way to track of compiler checksum, 
so you can only talk to the server,
with the same version you compiled for the Server,
they are tied together at compile time,
each knows the checksum of the other at all times,
so neither can be overwritten by a virus,
or anyone wanting to use the App to hack into a WeBook site.


## The Security System

The Project uses other GitHub Projects,
all list here, and I have to trust their code,
and QtLabs is trustworthy in my own opinion,
I use their code, so I have trust it,
and its easy to say I should read all the code,
but its in development and changes all the time,
as is Qt itself,
so I can only speak about my Code,
and I do not have any hidden code or back-doors,
malware, viruses, or any type of code other than just the code it needs to do its job,
so I will over comment the code,
so you know what all the code does by reading the comments.

I will have Tracking Capabilities built into this code,
but its job is to track where the code was compiled,
and is Fork Friendly,
you Fork it, it now uses your credentials,
your Travis and AppVeyor accounts.

The Security System has nothing to do with the Code,
the reason is simple, if the Code was in charge of Security,
someone could Hack it to break the Security,
so instead, I have my own Home Grown Security System,
I call WeBookSecurity.

WeBookSecurity is a System,
it starts by installed [fail2ban](https://github.com/fail2ban/fail2ban) on the server,
and monitoring it, tweaking it,
and HaProxy, so it serves as the Man In The Middle,
and helps prevent others from trying,
the concept is simple, fail2ban will lock out any attempt to hack in,
it has nothing to do with WeBook, so its external Security,
and WeBook just makes sure its doing its job,
and HaProxy means I do not have to deal with any HTTP request directly,
I only listen for HaProxy to send me traffic,
so it vets all traffic before it gets to me,
this helps prevent a DoS attack,
so by using external apps, 
and monitoring them, I can ensure security,
and I use Monit, to ensure all these apps are always running on the server.

I use [maldet](https://github.com/waja/maldetect) and [clamav](https://github.com/Cisco-Talos/clamav-devel) for scanning all incoming files.

I can use HaProxy to Authenticate all Clients,
and even filter by specific IP addresses.

By Removing the Security from this App,
it prevents it from being used to hack sites with,
even with the code, the rules it uses even when Forked,
as long as they do not change the code,
and GitHub makes it easy to find out what they changed,
but even if they wrote their own client server,
that was designed only to hack,
all they could hack is their own accounts,
since a legitimate site, would have better security.

I use a 3 Key System, the Admin of the VPS,
will download and install WeBookClient,
it will install WeBookServer,
and this is the only way this works,
because the two are matched at the point of installation,
and you cannot break this bound once you start using the system,
if your machine crashes, you much download the installer from the GitHub Server,
it will then reestablish this Trust,
but you will have to re-authenticate.

A lot of code goes into establishing a Trust between Client and Server,
so it will be hard to break, even knowing how I did it, 
and why I am telling you, so you will think twice about it, 
and knowing how it works, makes it clear why it works,
and why it will be hard to hack.

I use Travis or AppVeyor to create all the apps,
so I can track the public checksum to ensure it was downloaded from that account,
and it goes by account, its not tied to my WeBook Accounts, that is only if you want to use my Version of it,
you can Fork it, create your own accounts, and not even need to change any code,
it is designed to be Forked,
I will give more details about this later,
but the App handles most of this.

The Admin creates an SSH key, and they are the only one that has access to it,
anyone that uses your Account, as a Domain Controller,
you the Admin, are that Domain Controller,
you add all your Clients,
so this is how it works, 
you get a new Employee, or a new Step member or in-law in your Family,
and you send them a link, it has an Embedded Code in it,
only the Admin knows this Code, so the Admin,
and just to be clear, this is a screen in WeBookClient, 
its a Wizard called Add new user,
you can add a new SSH key for each user, 
and control the keys,
and you have the option to make it a 1 to 3 Step Process,
first Step is Email, WeBook sends the new user a code,
they reply, and that code is deleted, and void,
a one ticket ride,
now a 2 Step can be any order, Phone Voice via Key Pad entry, Voice Commands, SMS, or Snail Mail,
as well as a few I will add later,
and you can pick a 3 Step process, this may require in person check,
or may require more then one step, maybe all of them up to in person,
the options are limitless.

The Admin uses WeBookClient to manage a WeBook, 
and they can have multiple WeBooks to manage,
and everyone of those WeBooks has its on Users Account,
and it also has Linked Accounts, so you can have users in multiple WeBooks,
and keep in mind that WeBookServer, does not have to publish pages to the Public,
and why Fork it is an Option, without having to maintain the code,
they can ensure its safe to use before using the latest version,
they are not forced to do updates, and can update only what they want, 
even line by line, character by character, full git control is built into WeBook,
its actually a GitHub App when you think about,
and its geared mainly for Paid Accounts, meaning Private Accounts,
and its where the majority of Forks will take place.

I do not work for GitHub, nor do I get any kick backs,
what I do understand is how it works,
and Money is the Key factor, 
but its more than that when it comes to Security, 
and this is why.

If you have a Private GitHub Account, you can control who can download your App,
not that it matters, the Apps can be hacked, but only Apps you have compiled at Travis and AppVeyor,
where you will also want Paid Accounts, again, only for Security,
I use the keys they publish in a Public Free Account,
so if you want real Security,
you have to Pay for it, not to me, or this Project,
it is Open Source,
but you have to Pay GitHub for a Private Account,
and I do recommend this, you now have full control over your Repository,
I have to keep this site Public, so no reason to have it Private,
but once I start using it, I will get a Private Account,
its Money worth paying GitHub for,
as well as Travis https://travis-ci.org/ or AppVeyor https://www.AppVeyor.com/, keeping those checksum values private is worth the money.

Privacy is the Key to Security, so you get what you pay for, and keep in mind I make no money by recommending this,
in fact, it will cost me the same amount it cost you, if I did get a kickback, you would know it, I would put a link to it, 
the above have no Tracking information embedded into them, and if they did, I would let you know, 
privacy cost all of us the same amount of money, no kick backs.

So how does this Security System works, well its Simple,
and you know that KISS thing, not the I want to Rock and Roll All Knight, 
and you know what all day, but the Keep It Simple Stupidly,
and that means, if Stupid People set it up, you want to make sure their covered,
so now it comes down to how this Security System Works.

### Security System Work Flow

1. Data Security: Private GitHub, Travis, and AppVeyor Accounts
2. X Step Key Authentication, you pick the X Value
3. The Actual Personal Access Key

I have covered steps 1 and 2, but 3 is the real deal, 
and this all depends on your need for Trust.

Let me give you a few scenarios, first is a friend or family,
they want to be part of your WeBook, so they ask you,
or maybe you asked them,but trust goes both ways,
so you tell them no problem, go here and download WeBook,
or download my WeBook Fork, its sounds better does it,
I will have to sell T-Shirts, bumper stickers, magnets and such with "Forked WeBook" on it,
now once they installed it, and have it up and running,
you have them do this, 
shut it down, 
and edit the Menu item you used to launch it,
now there are a few options, and this is only one,
so if you do not like it, I will give you at least 3,
the first is the Edit the Menu Item, and pass in the Arguments on the Command Line,
this is as secure the device, because anyone that logs in can open it without a user name or password,
and putting one in, which you have that option, is bypassed by this option,
so this option, by the way, is the one I use, not the best, but if you can log into my Linux machine, 
or its already logged in, and you have access to it, 
there is little in the way of Security that will help you at this point, 
so let us think about other options.

1. Bluetooth Security Device
2. Smart Card/USB.
3. Encrypted Menu/Launcher and Key File

First for Bluetooth Security Devices, my Phonak Hearing Aids will work, 
any device you have that Qt can detect its Proximity can be used,
this means you must be paired with with the device,
so it makes no difference what device it is, as long as you paired to it, 
the Wizard dose not auto Pair anything, you have to manually pick Add Bluetooth Device.
Do not use any device you do not keep on you, proximity is your only Security in this case.
I suggest buying a very reliable small device that you can use for some person use, 
like a smart device you cannot live without, some people can use their Smart Injectors, 
or Smart Glasses, there are a lot of Bluetooth devices out there.

Smart Card/USB, now there are other devices than this, so expand on them here,
personally I would go with RFID Tags that have a reliable proximity range.
I keep this at number 2 only because most users cannot afford this method,
method 3 we will get into but its free, so this is in terms of how much it cost.
USB is only good if you have it on a strap that has no way of leaving your body, 
normally it is on a security key belt, this is a belt you wear, 
it has a metal case with a key ring on it, and when you pull it,
out comes a cable, how long depends on the model you bought,
these are easy to get, just shop around, you just need a USB that attaches to a key chain.
The Card can be anything, personally I would go with RFID technology, 
they even have tags you can buy online to tag your pets, children, or yourself, 
in fact, you can go to a doctor and have on put into you,
and no, this is not the Mark of the Beast,
its real Security.

Encrypted Menu/Launcher and Key File, this is the cheapest,
and how you implement it is up to you,
I do not hard code it for reason, if no one knows, its more secure.
One way is to make an ini file,
I will make this part of the App, so you do not actually need to make,
just pick Wizard make Security Key,
its built in, and it uses the Checksum as trust,
to mate the Client to the Server,
so you click the button, it gives you all the options on how to get the key to the user,
now the User takes the key, and they run the Wizard I have a Key to a Domain Controller,
and it knows what to do with it, end of point, next time they are logged in,
you have a way from the Controller side or WeBookServer,
to control how your access is Granted,
now this can lead into some heated conversations,
and I will not go there, this is not my concept,
but simple protocols for security,
say I am the Government, and I have a lot of Employees,
I can personally fit a profile that is the least evasive for all my employees,
by allowing them to chose the active method, 
for example, they want to use a RFID you ware, or USB you have to use a Key chain to operate,
which is good for cars, getting into your house,
but if they come to work, and cannot get in, they need a way to run a Smart App,
and that is where WeBookMobile comes in, it allows you to change your active mode of Authentication,
to another approved method, or the company can include more than one way to Authenticate.

Since there is no way anyone can ever guess at the credentials used,
since they come externally, and if encrypted, cannot be decrypted without the keys,
and one of the is Compiled into the WeBook app, and its only part of the 3 key system.

There is no web login or web interface to this app,
it does have the option to publish to a VPS, Server, or Local Folder,
and anyone that has Access to it, can have access to read only,
there is no web app to alter the site.

Even if you have the code to the App, and its Open Source,
and the whole reason is so people will,
once you start to use it, you figure out that its just enforcing good security.
The App itself only passes the Authentication codes through it,
and it can do AES encryption and decryption on that data,
using the checksum the App and Server where compiled at, 
along with some metadata that can be used to verify that these two apps are mated,
and why its important to have Private Accounts,
and why you should Fork it.

The Apps cannot be used to hack a site, but understand this,
who ever you given keys to, can alter what parts of the book you allow them too,
if none, then you are safe, the Domain Controllers Admin controls permissions,
and can revoke keys, and backups can restore data that they may have corrupted,
but a diff can tell them what they did change in real-time. 

## Fork it

Why Fork it is a question that is easy to answer,
if I am an individual, I want the maximum security I can get, 
if I am a Corporation or Government, and they are my Targeted users,
they will demand a higher standard of Security,
and Code Quality, so I will have to write Test Code for every function,
I will talk about Code Systems,
but the reason to Fork is for Control,
you do not have to modify the Code,
but you do want full control over how its compiled,
and you want people to go to your site, and use your Fork,
if you are a Company, its employees can only use your Fork,
and ideally everyone will either improve it, or just use it,
I do not care personally, and why I encourage you to Fork it.

WeBookClient gives you an upstream update Button Option,
and it will merge changes you made, with the upstream,
by selecting keep my changes, you do not overwrite yours,
if you made changes you do not like, and want to go with the current master,
you can select overwrite all, its all an Wizard that helps you make this decision,
and I am big on using Wizards.

You can use the WeBook Version, and it will just work.

## 3rd Party Apps

I compile these in, instead of using them as a Library, to make the app more compact.

I will try to keep third party apps to a minium, Qt is a given, and QtLabs projects that could become Qt are a given,
one that I am going to use is called QtService, it will give me the ability to write WeBookServer as a Daemon in Unix,
and as a Windows Service in Windows,
which allow you to control it with service calls to start, stop, enable, disable, status, and more.

* QtHttpServer: https://github.com/qt-labs/qthttpserver
* QtService:    https://github.com/qtproject/qt-solutions/tree/master/qtservice
* Qt-AES:       https://github.com/bricke/Qt-AES

The App will use git to sync these apps with WeBook at build time,
so all the apps should be up to date at build time, I do a pull for each repository,
if it fails to build, I have known good version I can fall back to,
this is all automated into the software,
you make an entry for the usability or rating of your current build,
you can log issues, and track them with WeBook,
for the Fork from which it came,
and even push those back to the origin,
which is this WeBook Project,
or any Fork that may become more popular,
so I can do opt-in issue tracking for Forks.

## Notes

This is on Arch Linux or Manjaro


```
qmake -query
QT_SYSROOT:
QT_INSTALL_PREFIX:/usr
QT_INSTALL_ARCHDATA:/usr/lib/qt
QT_INSTALL_DATA:/usr/share/qt
QT_INSTALL_DOCS:/usr/share/doc/qt
QT_INSTALL_HEADERS:/usr/include/qt
QT_INSTALL_LIBS:/usr/lib
QT_INSTALL_LIBEXECS:/usr/lib/qt/libexec
QT_INSTALL_BINS:/usr/bin
QT_INSTALL_TESTS:/usr/tests
QT_INSTALL_PLUGINS:/usr/lib/qt/plugins
QT_INSTALL_IMPORTS:/usr/lib/qt/imports
QT_INSTALL_QML:/usr/lib/qt/qml
QT_INSTALL_TRANSLATIONS:/usr/share/qt/translations
QT_INSTALL_CONFIGURATION:/etc/xdg
QT_INSTALL_EXAMPLES:/usr/share/doc/qt/examples
QT_INSTALL_DEMOS:/usr/share/doc/qt/examples
QT_HOST_PREFIX:/usr
QT_HOST_DATA:/usr/lib/qt
QT_HOST_BINS:/usr/bin
QT_HOST_LIBS:/usr/lib
QMAKE_SPEC:linux-g++
QMAKE_XSPEC:linux-g++
QMAKE_VERSION:3.1
QT_VERSION:5.14.2
```

This is on Travis

```
QT_SYSROOT:
QT_INSTALL_PREFIX:/opt/qt514
QT_INSTALL_ARCHDATA:/opt/qt514
QT_INSTALL_DATA:/opt/qt514
QT_INSTALL_DOCS:/opt/qt514/doc
QT_INSTALL_HEADERS:/opt/qt514/include
QT_INSTALL_LIBS:/opt/qt514/lib
QT_INSTALL_LIBEXECS:/opt/qt514/libexec
QT_INSTALL_BINS:/opt/qt514/bin
QT_INSTALL_TESTS:/opt/qt514/tests
QT_INSTALL_PLUGINS:/opt/qt514/plugins
QT_INSTALL_IMPORTS:/opt/qt514/imports
QT_INSTALL_QML:/opt/qt514/qml
QT_INSTALL_TRANSLATIONS:/opt/qt514/translations
QT_INSTALL_CONFIGURATION:/opt/qt514/etc/xdg
QT_INSTALL_EXAMPLES:/opt/qt514/examples
QT_INSTALL_DEMOS:/opt/qt514/examples
QT_HOST_PREFIX:/opt/qt514
QT_HOST_DATA:/opt/qt514
QT_HOST_BINS:/opt/qt514/bin
QT_HOST_LIBS:/opt/qt514/lib
QMAKE_SPEC:linux-g++
QMAKE_XSPEC:linux-g++
QMAKE_VERSION:3.1
QT_VERSION:5.14.1
```


```
git clone --recursive --recurse-submodules https://github.com/qt-labs/qthttpserver.git

git submodule add https://github.com/qt-labs/qthttpserver.git 3rdparty/qhttpserver

sudo systemctl start 
/path/to/service --backend standard

sudo /mnt/qnap-light-wizzard/workspace/build/QtService/WeBookServer/debug/WeBookServer --backend standard
sudo /mnt/qnap-light-wizzard/workspace/build/QtService/WeBookServer/debug/WeBookServer --backend systemd
sudo /mnt/qnap-light-wizzard/workspace/build/QtService/WeBookServer/debug/WeBookServer --backend systemd reload
sudo /mnt/qnap-light-wizzard/workspace/build/QtService/WeBookServer/debug/WeBookServer --backend systemd stop
sudo /mnt/qnap-light-wizzard/workspace/build/QtService/WeBookServer/debug/WeBookServer --backend systemd start
 
 port 6627


qDebug() << Q_FUNC_INFO;

```

## List of 3rd Party Projects used

Internal:

* [QtLabs QtHttpServer](https://github.com/qt-labs/qthttpserver)
* [QtLabs QtService](https://github.com/qtproject/qt-solutions/tree/master/qtservice)
* [Qt-AES](https://github.com/bricke/Qt-AES)

External:

* [fail2ban](https://github.com/fail2ban/fail2ban)
* [HAProxy](https://github.com/haproxy/haproxy)
* [Monit](https://github.com/arnaudsj/monit)
* [maldet](https://github.com/waja/maldetect)
* [clamav](https://github.com/Cisco-Talos/clamav-devel)

Services:

* [Travis](https://travis-ci.org/)
* [AppVeyor](https://www.AppVeyor.com/)

Windows:

[PowerShell](https://docs.microsoft.com/en-us/powershell/scripting/overview?view=powershell-7)
[Cygwin](https://www.cygwin.com/)

## Progress

This is a Work in Progress...

## Ending Remarks

Thanks, Jeffrey Scott Flesher, AKA the Light Wizzard in the Flesh

## End of File, you can stop reading now...
