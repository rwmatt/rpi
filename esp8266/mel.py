import network

import settings

def do_connect():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(settings.wifi_essid, settings.wifi_password)
        while not sta_if.isconnected():
            pass
	print('network config:', sta_if.ifconfig())

# http://my-small-projects.blogspot.com/2015/05/arduino-checking-for-british-summer-time.html
def IsBST(yr, imonth, iday, hr):
    #January, february, and november are out.
    if (imonth < 3 or imonth > 10): return False
    #April to September are in
    if (imonth > 3 and imonth < 10): return True

    #find last sun in mar and oct - quickest way I've found to do it
    #last sunday of march
    lastMarSunday =  (31 - (5* yr /4 + 4) % 7)
    #last sunday of october
    lastOctSunday = (31 - (5 * yr /4 + 1) % 7)
        
    #In march, we are BST if is the last sunday in the month
    if (imonth == 3):      
      if ( iday > lastMarSunday):
        return True
      if ( iday < lastMarSunday):
        return False
      
      if (hr < 1):
        return False
              
      return True; 
  
    
    #In October we must be before the last sunday to be bst.
    #That means the previous sunday must be before the 1st.
    if (imonth == 10):

      if ( iday < lastOctSunday):
        return True
      if ( iday > lastOctSunday):
        return False
      
      if (hr >= 1):
        return false;
        
      return True 
    

def adjustBST(yr, imonth, iday, hr):
    if not IsBST(yr, imonth, iday, hr):
        return imonth, iday, hr
    daysInMonth = { 3:31, 4:30, 5:31, 6:30,  7:31, 8:31, 9:30, 10:31 }
    hr += 1
    if hr == 24:
        hr = 1
        iday += 1
        if iday> daysInMonth[imonth]:
            iday = 1
            imonth +=1
    return imonth, iday, hr
              
#print(adjustBST(2019, 5, 31, 23))
#print(adjustBST(2019, 2, 24, 20))
