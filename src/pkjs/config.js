module.exports = [
  {
    "type": "heading",
    "defaultValue": "Time Minder: Settings",
    "size": 3
  },
  
  
  
  
  
  {
    "type": "section",
    "items": [
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
        "defaultValue": "000000",
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
        "defaultValue": "ffffff",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "AlertForegroundColor",
        "defaultValue": "005500",
        "label": "Text Color"
      }
    ]
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
        "type": "toggle",
        "messageKey": "AlertsEnabled",
        "label": "Enable Alerts",
        "defaultValue": true
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
        "type": "slider",
        "messageKey": "AlertStartHour",
        "label": "Alert Start Hour",
        "description": "E.g., 9 = 0900 (9:00am), 22 = 2200 (10:00pm), etc.",
        "defaultValue": 9,
        "min": 0,
        "max": 24,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "AlertEndHour",
        "label": "Alert End Hour",
        "defaultValue": 22,
        "min": 0,
        "max": 24,
        "step": 1
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
    "defaultValue": "Save Settings"
  }
];