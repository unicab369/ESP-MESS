# Raspi Setup
1, Download Raspberrypi Imager: https://www.raspberrypi.com/software/

2, Flash OS on micro SD card:
    - Format micro sd card
    - Run Imager software
    - Select Operating system
    - Select Storage (micro sd card)

3, Update settings and flash SD card:
    - Set hostname *pi_name*.local
    - Check Enable SSH, Use password authentication
    - Set *username* and password. eg: admin/password
    - Check Configure wireless LAN
    - Enter wifi SSID and password
    - Select Country and local time
    - Save and Write to SD card

4, Running Raspi:
    - Remove the card when flashing complete and put on Raspi
    - It takes about 5-6 minutes for the OS to startup

5, Connect to Raspi:
    - On another device, run terminal 
    - ssh into device using command: ssh *username*@*pi_name*.local
    - if not working try: ssh *username*@*raspi_IP_addr* (check wifi router)
    - allow root access command: sudo su -

6, Status commands:
    - free -m OR free -h
    - top

*Note: to handle WARNING REMOTE HOST IDENTIFICATION HAS CHANGED
go to the target path (eg: /User/*userName*/.ssh/known_hosts) file and delete the content of the file. Then try again.

6, Configure VNC:
    - Run command: raspi-config >> 3 Interface Options >> VNC >> Enable <yes>
    - 2 Display Options >> D5 VNC Resolution >> 800 x 600 >> <yes> 
        *Note: we use 800 x 600 to allow faster transmission speed over VNC
    - Enable Desktop login: 1 System Options >> S5 Boot/Auto Login >> B3 Desktop
    - Reboot

7, Running VNC Client: (VNCviewer or TeamViewer)
    - Download VNCViewer: https://www.realvnc.com/en/connect/download/viewer/
    - Create account and login
    - Open VNC >> Enter the Raspi IP address and connect
    



