Time Minder
===========

_(Name subject to change, because that name seems terrible...)_

A simple, clean, digital watchface for [Pebble](https://www.pebble.com/) which is intended to help the wearer be more aware of the passing of time.

Currently, it does the following:

* Vibes every 15 mins, with a vibe pattern that's sufficiently aggressive that it's hard to ignore.
* Basic implementation of quiet time: Vibe is only enabled from 9:00am to 10:00pm (0900-2200).
* Displays time and date.
* Displays minutes remaining til next alarm. 

TODO:

* Add battery indicator.
* Add bluetooth indicator.
* Add user configuration for alarm, complications, and colors. (Currently, nothing is configurable.)
* A bunch of testing, debugging, and combing for memory leaks, probably. (I haven't noticed any glaring issues yet, but _complacency is complicity!)_
* Release first version to the Pebble app store.
