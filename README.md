Time Minder
===========

_(Name subject to change, because that name seems terrible...)_

A simple, clean, digital watchface for [Pebble](https://www.pebble.com/) which is intended to help the wearer be more aware of the passing of time.

Currently, it does the following:

* Vibes every 15 mins, with a vibe pattern that's sufficiently aggressive that it's hard to ignore.
* Basic implementation of quiet time: Vibe is only enabled from 9:00am to 10:00pm (0900-2200).
* Displays time and date.
* Displays minutes remaining til next alarm.



## Screenshots

### Pebble Time, emulated:

![Watch in Basalt emulator](https://raw.githubusercontent.com/nwinant/time-minder/master/assets/screenshots/basalt.png "Watch in Basalt emulator")
![Watch in Basalt emulator, during quarter-hour alarm](https://raw.githubusercontent.com/nwinant/time-minder/master/assets/screenshots/basalt-alarm.png "Watch in Basalt emulator, during quarter-hour alarm")

### Pebble Round, emulated:

![Watch in Chalk emulator](https://raw.githubusercontent.com/nwinant/time-minder/master/assets/screenshots/chalk.png "Watch in Chalk emulator")
![Watch in Chalk emulator, during quarter-hour alarm](https://raw.githubusercontent.com/nwinant/time-minder/master/assets/screenshots/chalk.png "Watch in Chalk emulator, during quarter-hour alarm")

### Pebble Classic, emulated:

![Watch in Aplite emulator](https://raw.githubusercontent.com/nwinant/time-minder/master/assets/screenshots/aplite.png "Watch in Aplite emulator")
![Watch in Aplite emulator, during quarter-hour alarm](https://raw.githubusercontent.com/nwinant/time-minder/master/assets/screenshots/aplite-alarm.png "Watch in Aplite emulator, during quarter-hour alarm")



## TODO

* Add battery indicator.
* Add bluetooth indicator.
* Add user configuration for alarm, complications, and colors. (Currently, nothing is configurable.)
* A bunch of testing, debugging, and combing for memory leaks, probably. (I haven't noticed any glaring issues yet, but _complacency is complicity!)_
* Release first version to the Pebble app store.
