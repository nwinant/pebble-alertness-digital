module.exports = [
  {
    "type": "heading",
    "defaultValue": "Digital Alertness: Settings",
    "size": 3
  },
  
  
  
  
  
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Quick Settings"
      },
      {
        "type": "toggle",
        "messageKey": "AlertsEnabled",
        "label": "Allow Vibrating Alerts",
        "defaultValue": true
      },
      {
        "type": "heading",
        "defaultValue": "Switching this off will mute all watchface vibrations.",
        "size": 6
      },
    ]
  },
  
  
  {
    "type": "submit",
    "defaultValue": "Save All Settings"
  },
  
  
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Layout"
      },
      {
        "type": "heading",
        "defaultValue": "Time Colors"
      },
      {
        "type": "heading",
        "defaultValue": "Upper half of the screen, displaying the time:",
        "size": 6
      },
      {
        "type": "color",
        "messageKey": "MainBackgroundColor",
        "defaultValue": "000000",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "MainForegroundColor",
        "defaultValue": "ffffff",
        "label": "Text Color"
      },
      
      
      {
        "type": "heading",
        "defaultValue": "Complication Colors"
      },
      {
        "type": "heading",
        "defaultValue": "Lower half of the screen, displaying extra info (date, minutes remaining until next alert, etc.):",
        "size": 6
      },
      {
        "type": "color",
        "messageKey": "ComplicationsBackgroundColor",
        "defaultValue": "ff0000",
        "label": "Background Color",
      },
      {
        "type": "color",
        "messageKey": "ComplicationsForegroundColor",
        "defaultValue": "ffffff",
        "label": "Text Color"
      },
      
      
      {
        "type": "heading",
        "defaultValue": "Alert Colors"
      },
      {
        "type": "heading",
        "defaultValue": "During the minute when an alert is active, the <em>entire</em> screen will change to this color scheme:",
        "size": 6
      },
      {
        "type": "color",
        "messageKey": "AlertBackgroundColor",
        "defaultValue": "550000",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "AlertForegroundColor",
        "defaultValue": "ffffff",
        "label": "Text Color"
      },
      
      
      {
        "type"        : "heading",
        "defaultValue": "Fonts"
      },
      {
        "type"        : "select",
        "messageKey"  : "TimeFont",
        "label"       : "Time Font",
        "defaultValue": "time_comfortaa",
        "options": [
          {
            "label": "Comfortaa",
            "value": "time_comfortaa"
          },
          {
            "label": "Dinen",
            "value": "time_dinen"
          },
          {
            "label": "Bitham",
            "value": "time_bitham"
          },
          {
            "label": "Roboto",
            "value": "time_roboto"
          }
        ]
      }
    ]
  },
    
    
  {
    "type": "submit",
    "defaultValue": "Save All Settings"
  },
  
  
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Alerts"
      },
      {
        "type": "heading",
        "defaultValue": "Repeating alerts that can trigger every hour, every 30 mins, every 15 mins, etc.",
        "size": 6
      },
      {
        "type": "select",
        "messageKey": "AlertFrequency",
        "label": "Alert Frequency",
        "defaultValue": "15",
        "options": [
          {
            "label": "Every hour",
            "value": "60"
          },
          {
            "label": "Every 30 mins",
            "value": "30"
          },
          {
            "label": "Every 20 mins",
            "value": "20"
          },
          {
            "label": "Every 15 mins",
            "value": "15"
          },
          {
            "label": "Every 10 mins",
            "value": "10"
          },
          {
            "label": "Every 5 mins",
            "value": "5"
          },
          {
            "label": "Every 3 mins",
            "value": "3"
          },
          {
            "label": "Every minute (!!!)",
            "value": "1"
          },
        ]
      },
      {
        "type": "select",
        "messageKey": "AlertVibePattern",
        "label": "Vibe Pattern",
        "defaultValue": "very_long",
        "options": [
          {
            "label": "Very long",
            "value": "very_long"
          },
          {
            "label": "Short",
            "value": "short"
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "AlertStartHour",
        "label": "Alert Start Hour",
        "defaultValue": "9",
        "options": [
          {
            "label": "Midnight",
            "value": "0"
          },
          {
            "label": "1:00 am",
            "value": "1"
          },
          {
            "label": "2:00 am",
            "value": "2"
          },
          {
            "label": "3:00 am",
            "value": "3"
          },
          {
            "label": "4:00 am",
            "value": "4"
          },
          {
            "label": "5:00 am",
            "value": "5"
          },
          {
            "label": "6:00 am",
            "value": "6"
          },
          {
            "label": "7:00 am",
            "value": "7"
          },
          {
            "label": "8:00 am",
            "value": "8"
          },
          {
            "label": "9:00 am",
            "value": "9"
          },
          {
            "label": "10:00 am",
            "value": "10"
          },
          {
            "label": "11:00 am",
            "value": "11"
          },
          {
            "label": "Noon",
            "value": "12"
          },
          {
            "label": "1:00 pm",
            "value": "13"
          },
          {
            "label": "2:00 pm",
            "value": "14"
          },
          {
            "label": "3:00 pm",
            "value": "15"
          },
          {
            "label": "4:00 pm",
            "value": "16"
          },
          {
            "label": "5:00 pm",
            "value": "17"
          },
          {
            "label": "6:00 pm",
            "value": "18"
          },
          {
            "label": "7:00 pm",
            "value": "19"
          },
          {
            "label": "8:00 pm",
            "value": "20"
          },
          {
            "label": "9:00 pm",
            "value": "21"
          },
          {
            "label": "10:00 pm",
            "value": "22"
          },
          {
            "label": "11:00 pm",
            "value": "23"
          },
          {
            "label": "Midnight",
            "value": "24"
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "AlertEndHour",
        "label": "Alert End Hour",
        "defaultValue": "22",
        "options": [
          {
            "label": "Midnight",
            "value": "0"
          },
          {
            "label": "1:00 am",
            "value": "1"
          },
          {
            "label": "2:00 am",
            "value": "2"
          },
          {
            "label": "3:00 am",
            "value": "3"
          },
          {
            "label": "4:00 am",
            "value": "4"
          },
          {
            "label": "5:00 am",
            "value": "5"
          },
          {
            "label": "6:00 am",
            "value": "6"
          },
          {
            "label": "7:00 am",
            "value": "7"
          },
          {
            "label": "8:00 am",
            "value": "8"
          },
          {
            "label": "9:00 am",
            "value": "9"
          },
          {
            "label": "10:00 am",
            "value": "10"
          },
          {
            "label": "11:00 am",
            "value": "11"
          },
          {
            "label": "Noon",
            "value": "12"
          },
          {
            "label": "1:00 pm",
            "value": "13"
          },
          {
            "label": "2:00 pm",
            "value": "14"
          },
          {
            "label": "3:00 pm",
            "value": "15"
          },
          {
            "label": "4:00 pm",
            "value": "16"
          },
          {
            "label": "5:00 pm",
            "value": "17"
          },
          {
            "label": "6:00 pm",
            "value": "18"
          },
          {
            "label": "7:00 pm",
            "value": "19"
          },
          {
            "label": "8:00 pm",
            "value": "20"
          },
          {
            "label": "9:00 pm",
            "value": "21"
          },
          {
            "label": "10:00 pm",
            "value": "22"
          },
          {
            "label": "11:00 pm",
            "value": "23"
          },
          {
            "label": "Midnight",
            "value": "24"
          }
        ]
      }
    ]
  },
  
      
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Miscellaneous"
      },
      
      
      
      {
        "type"        : "select",
        "messageKey"  : "DateFormat",
        "label"       : "Date Format",
        "defaultValue": "%a %m-%d",
        "options": [
          {
            "label": "Mon 01-23",
            "value": "%a %m-%d"
          },
          {
            "label": "Mon 01/23",
            "value": "%a %m/%d"
          },
          {
            "label": "Mon Jan 9",
            "value": "%a %b %e"
          },
          {
            "label": "2017-01-23",
            "value": "%Y-%m-%d"
          }
        ]
      },
      
      
      
      {
        "type": "toggle",
        "messageKey": "ShowConnectionStatus",
        "label": "Show bluetooth connection status",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "ShowBatteryStatus",
        "label": "Display battery %",
        "defaultValue": true
      }
    ]
  },
  
  
  {
    "type": "submit",
    "defaultValue": "Save All Settings"
  }
];