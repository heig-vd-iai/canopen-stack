# Object Dictionary

## Module

| Index | name                                                                                       |
| ----- | ------------------------------------------------------------------------------------------ |
| 1000  | [Device type](modules//#device-type)                                                       |
| 1001  | [Error register](modules//#error-register)                                                 |
| 1002  | [Manufacturer status register](modules//#manufacturer-status-register)                     |
| 1003  | [Pre-defined error field](modules//#pre-defined-error-field)                               |
| 1010  | [Store parameters](modules//#store-parameters)                                             |
| 1011  | [Restore default parameters](modules//#restore-default-parameters)                         |
| 1017  | [Producer hearbeat time](modules//#producer-hearbeat-time)                                 |
| 1018  | [Identity object](modules//#identity-object)                                               |
| 1019  | [Synchronous counter overflow value](modules//#synchronous-counter-overflow-value)         |
| 1029  | [Error behavior](modules//#error-behavior)                                                 |
| 1400  | [Receive PDO 1 communication parameter](modules//#receive-pdo-1-communication-parameter)   |
| 1401  | [Receive PDO 2 communication parameter](modules//#receive-pdo-2-communication-parameter)   |
| 1402  | [Receive PDO 3 communication parameter](modules//#receive-pdo-3-communication-parameter)   |
| 1403  | [Receive PDO 4 communication parameter](modules//#receive-pdo-4-communication-parameter)   |
| 1600  | [Receive PDO 1 mapping parameter](modules//#receive-pdo-1-mapping-parameter)               |
| 1601  | [Receive PDO 2 mapping parameter](modules//#receive-pdo-2-mapping-parameter)               |
| 1602  | [Receive PDO 3 mapping parameter](modules//#receive-pdo-3-mapping-parameter)               |
| 1603  | [Receive PDO 4 mapping parameter](modules//#receive-pdo-4-mapping-parameter)               |
| 1800  | [Transmit PDO 1 communication parameter](modules//#transmit-pdo-1-communication-parameter) |
| 1801  | [Transmit PDO 2 communication parameter](modules//#transmit-pdo-2-communication-parameter) |
| 1802  | [Transmit PDO 3 communication parameter](modules//#transmit-pdo-3-communication-parameter) |
| 1803  | [Transmit PDO 4 communication parameter](modules//#transmit-pdo-4-communication-parameter) |
| 1a00  | [Transmit PDO 1 mapping parameter](modules//#transmit-pdo-1-mapping-parameter)             |
| 1a01  | [Transmit PDO 2 mapping parameter](modules//#transmit-pdo-2-mapping-parameter)             |
| 1a02  | [Transmit PDO 3 mapping parameter](modules//#transmit-pdo-3-mapping-parameter)             |
| 1a03  | [Transmit PDO 4 mapping parameter](modules//#transmit-pdo-4-mapping-parameter)             |

## Module current-loop

| Index | name                                                                     |
| ----- | ------------------------------------------------------------------------ |
| 2100  | [Current Control](modules/current-loop/#current-control)                 |
| 2101  | [dc-link voltage](modules/current-loop/#dc-link-voltage)                 |
| 2102  | [dc-link min-max voltage](modules/current-loop/#dc-link-min-max-voltage) |
| 2103  | [theta invert](modules/current-loop/#theta-invert)                       |
| 2104  | [Theta source](modules/current-loop/#theta-source)                       |
| 2105  | [Current demand](modules/current-loop/#current-demand)                   |
| 6074  | [Torque demand](modules/current-loop/#torque-demand)                     |
| 6079  | [DC link voltage](modules/current-loop/#dc-link-voltage)                 |

## Module velocity-loop

| Index | name                                                                              |
| ----- | --------------------------------------------------------------------------------- |
| 2200  | [Velocity Control](modules/velocity-loop/#velocity-control)                       |
| 2201  | [velocity-loop prescaler](modules/velocity-loop/#velocity-loop-prescaler)         |
| 2202  | [Velocity encoder inverted](modules/velocity-loop/#velocity-encoder-inverted)     |
| 2203  | [Following velocity window](modules/velocity-loop/#following-velocity-window)     |
| 2204  | [Following velocity time out](modules/velocity-loop/#following-velocity-time-out) |
| 2205  | [Velocity error](modules/velocity-loop/#velocity-error)                           |
| 606b  | [Velocity demand value](modules/velocity-loop/#velocity-demand-value)             |
| 606c  | [Velocity actual value](modules/velocity-loop/#velocity-actual-value)             |
| 606d  | [Velocity window](modules/velocity-loop/#velocity-window)                         |
| 606e  | [Velocity window time](modules/velocity-loop/#velocity-window-time)               |
| 606f  | [Velocity threshold](modules/velocity-loop/#velocity-threshold)                   |
| 6070  | [Velocity threshold time](modules/velocity-loop/#velocity-threshold-time)         |
| 6090  | [Velocity sensor resolution](modules/velocity-loop/#velocity-sensor-resolution)   |
| 60f8  | [Max slippage](modules/velocity-loop/#max-slippage)                               |
| 60ff  | [target velocity](modules/velocity-loop/#target-velocity)                         |

## Module position-loop

| Index | name                                                                              |
| ----- | --------------------------------------------------------------------------------- |
| 2300  | [Position Control](modules/position-loop/#position-control)                       |
| 2301  | [Position encoder inverted](modules/position-loop/#position-encoder-inverted)     |
| 2302  | [Following fault window](modules/position-loop/#following-fault-window)           |
| 2303  | [Following fault time out](modules/position-loop/#following-fault-time-out)       |
| 2304  | [Position error](modules/position-loop/#position-error)                           |
| 6062  | [Position demand value](modules/position-loop/#position-demand-value)             |
| 6064  | [Position actual value](modules/position-loop/#position-actual-value)             |
| 6065  | [Following error window](modules/position-loop/#following-error-window)           |
| 6066  | [Following error time out](modules/position-loop/#following-error-time-out)       |
| 6067  | [Position window](modules/position-loop/#position-window)                         |
| 6068  | [Position window time](modules/position-loop/#position-window-time)               |
| 607a  | [Target position](modules/position-loop/#target-position)                         |
| 607c  | [Home offset](modules/position-loop/#home-offset)                                 |
| 608f  | [Position encoder resolution](modules/position-loop/#position-encoder-resolution) |

## Module profile-position

| Index | name                                                                         |
| ----- | ---------------------------------------------------------------------------- |
| 2350  | [trajectory filter pow](modules/profile-position/#trajectory-filter-pow)     |
| 605d  | [Halt option code](modules/profile-position/#halt-option-code)               |
| 607e  | [Polarity](modules/profile-position/#polarity)                               |
| 6081  | [Profile velocity](modules/profile-position/#profile-velocity)               |
| 6083  | [Profile acceleration](modules/profile-position/#profile-acceleration)       |
| 6084  | [Profile deceleration](modules/profile-position/#profile-deceleration)       |
| 60f2  | [Positioning option code](modules/profile-position/#positioning-option-code) |

## Module limits

| Index | name                                                                                         |
| ----- | -------------------------------------------------------------------------------------------- |
| 2400  | [Hard limit](modules/limits/#hard-limit)                                                     |
| 2401  | [Velocity limit](modules/limits/#velocity-limit)                                             |
| 2402  | [Max velocity without homing done](modules/limits/#max-velocity-without-homing-done)         |
| 2403  | [Max acceleration without homing done](modules/limits/#max-acceleration-without-homing-done) |
| 2404  | [Max deceleration without homing done](modules/limits/#max-deceleration-without-homing-done) |
| 2410  | [Force limit](modules/limits/#force-limit)                                                   |
| 6072  | [Max torque](modules/limits/#max-torque)                                                     |
| 6073  | [Max current](modules/limits/#max-current)                                                   |
| 607d  | [Software position limit](modules/limits/#software-position-limit)                           |
| 607f  | [Max profile velocity](modules/limits/#max-profile-velocity)                                 |
| 60c5  | [Max acceleration](modules/limits/#max-acceleration)                                         |
| 60c6  | [Max deceleration](modules/limits/#max-deceleration)                                         |

## Module homing

| Index | name                                                                   |
| ----- | ---------------------------------------------------------------------- |
| 2500  | [Homing current threshold](modules/homing/#homing-current-threshold)   |
| 2501  | [Homing move away distance](modules/homing/#homing-move-away-distance) |
| 6098  | [Homing method](modules/homing/#homing-method)                         |
| 6099  | [Homing speed](modules/homing/#homing-speed)                           |
| 609a  | [Homing acceleration](modules/homing/#homing-acceleration)             |
| 60e3  | [Supported homing methods](modules/homing/#supported-homing-methods)   |

## Module motor

| Index | name                                                              |
| ----- | ----------------------------------------------------------------- |
| 2600  | [Motor Pole Pairs](modules/motor/#motor-pole-pairs)               |
| 2601  | [Motor resistance](modules/motor/#motor-resistance)               |
| 2602  | [Motor inductance](modules/motor/#motor-inductance)               |
| 2603  | [Motor peak current](modules/motor/#motor-peak-current)           |
| 2604  | [Motor back emf constant](modules/motor/#motor-back-emf-constant) |
| 2605  | [I2t limit](modules/motor/#i2t-limit)                             |
| 2606  | [Torque constant](modules/motor/#torque-constant)                 |
| 6075  | [motor rated current](modules/motor/#motor-rated-current)         |
| 6076  | [motor rated torque](modules/motor/#motor-rated-torque)           |
| 6080  | [Max motor speed](modules/motor/#max-motor-speed)                 |

## Module control

| Index | name                                                                          |
| ----- | ----------------------------------------------------------------------------- |
| 2700  | [Regulation switch](modules/control/#regulation-switch)                       |
| 2701  | [Function generator parameter](modules/control/#function-generator-parameter) |
| 2702  | [Digital input](modules/control/#digital-input)                               |
| 2703  | [Digital output](modules/control/#digital-output)                             |

## Module monitoring

| Index | name                                                                                               |
| ----- | -------------------------------------------------------------------------------------------------- |
| 2800  | [Position encoder sin signal](modules/monitoring/#position-encoder-sin-signal)                     |
| 2801  | [Position encoder cos signal](modules/monitoring/#position-encoder-cos-signal)                     |
| 2802  | [Velocity encoder sin signal](modules/monitoring/#velocity-encoder-sin-signal)                     |
| 2803  | [Velocity encoder cos signal](modules/monitoring/#velocity-encoder-cos-signal)                     |
| 2804  | [Position encoder sin corrected signal](modules/monitoring/#position-encoder-sin-corrected-signal) |
| 2805  | [Position encoder cos corrected signal](modules/monitoring/#position-encoder-cos-corrected-signal) |
| 2806  | [Velocity encoder sin corrected signal](modules/monitoring/#velocity-encoder-sin-corrected-signal) |
| 2807  | [Velocity encoder cos corrected signal](modules/monitoring/#velocity-encoder-cos-corrected-signal) |
| 2808  | [Cycle time](modules/monitoring/#cycle-time)                                                       |
| 2809  | [Cycle time inner loop](modules/monitoring/#cycle-time-inner-loop)                                 |
| 2810  | [Cycle time outer loop](modules/monitoring/#cycle-time-outer-loop)                                 |
| 2811  | [Cycle time pdo](modules/monitoring/#cycle-time-pdo)                                               |
| 2812  | [I2t current value](modules/monitoring/#i2t-current-value)                                         |
| 2850  | [Trace signal](modules/monitoring/#trace-signal)                                                   |
| 2851  | [Trigger mode](modules/monitoring/#trigger-mode)                                                   |
| 2852  | [Manual trigger](modules/monitoring/#manual-trigger)                                               |
| 2853  | [Trigger value](modules/monitoring/#trigger-value)                                                 |
| 2854  | [Trigger delay ms](modules/monitoring/#trigger-delay-ms)                                           |
| 2855  | [Trace remaining length](modules/monitoring/#trace-remaining-length)                               |
| 5000  | [Trace a](modules/monitoring/#trace-a)                                                             |
| 5001  | [Trace b](modules/monitoring/#trace-b)                                                             |

## Module servo-press

| Index | name                                        |
| ----- | ------------------------------------------- |
| 3000  | [Led mode](modules/servo-press/#led-mode)   |
| 3001  | [Led color](modules/servo-press/#led-color) |

## Module turn-table

| Index | name                                         |
| ----- | -------------------------------------------- |
| 3002  | [Turn table](modules/turn-table/#turn-table) |

## Module force-loop

| Index | name                                             |
| ----- | ------------------------------------------------ |
| 3005  | [Force target](modules/force-loop/#force-target) |
| 3006  | [Force demand](modules/force-loop/#force-demand) |
| 3007  | [Force actual](modules/force-loop/#force-actual) |

## Module profile-force

| Index | name                                              |
| ----- | ------------------------------------------------- |
| 3008  | [Force slope](modules/profile-force/#force-slope) |

## Module force-mode

| Index | name                                                                         |
| ----- | ---------------------------------------------------------------------------- |
| 3010  | [force-mode start position](modules/force-mode/#force-mode-start-position)   |
| 3011  | [force-mode end position](modules/force-mode/#force-mode-end-position)       |
| 3012  | [force-mode contact speed](modules/force-mode/#force-mode-contact-speed)     |
| 3013  | [force-mode force threshold](modules/force-mode/#force-mode-force-threshold) |
| 3014  | [force-mode wait time](modules/force-mode/#force-mode-wait-time)             |
| 3015  | [force target](modules/force-mode/#force-target)                             |
| 3016  | [force-mode force time](modules/force-mode/#force-mode-force-time)           |

## Module insert-mode

| Index | name                                                                                  |
| ----- | ------------------------------------------------------------------------------------- |
| 3100  | [Insert start position](modules/insert-mode/#insert-start-position)                   |
| 3101  | [Insert end position](modules/insert-mode/#insert-end-position)                       |
| 3102  | [Insert contact speed](modules/insert-mode/#insert-contact-speed)                     |
| 3103  | [Insert contact force threshold](modules/insert-mode/#insert-contact-force-threshold) |
| 3104  | [Insert speed](modules/insert-mode/#insert-speed)                                     |
| 3105  | [Insert position](modules/insert-mode/#insert-position)                               |
| 3106  | [Insert move away position](modules/insert-mode/#insert-move-away-position)           |
| 3107  | [Insert force threshold](modules/insert-mode/#insert-force-threshold)                 |
| 3108  | [Insert position toleration](modules/insert-mode/#insert-position-toleration)         |
| 3109  | [Max force](modules/insert-mode/#max-force)                                           |

## Module weight-simulator

| Index | name                                                                                         |
| ----- | -------------------------------------------------------------------------------------------- |
| 3200  | [Weight simulator sleep position](modules/weight-simulator/#weight-simulator-sleep-position) |
| 3201  | [Weight simulator force trigger](modules/weight-simulator/#weight-simulator-force-trigger)   |
| 3202  | [Weight simulator wait time](modules/weight-simulator/#weight-simulator-wait-time)           |
| 3203  | [Weight simulator force target](modules/weight-simulator/#weight-simulator-force-target)     |

## Module music

| Index | name                                    |
| ----- | --------------------------------------- |
| 3500  | [Play music](modules/music/#play-music) |

## Module state-machine

| Index | name                                                                            |
| ----- | ------------------------------------------------------------------------------- |
| 603f  | [Error code](modules/state-machine/#error-code)                                 |
| 6040  | [Controlword](modules/state-machine/#controlword)                               |
| 6041  | [Statusword](modules/state-machine/#statusword)                                 |
| 605a  | [Quick stop option code](modules/state-machine/#quick-stop-option-code)         |
| 6060  | [Modes of operation](modules/state-machine/#modes-of-operation)                 |
| 6061  | [Modes of operation display](modules/state-machine/#modes-of-operation-display) |
| 6085  | [Quick stop deceleration](modules/state-machine/#quick-stop-deceleration)       |
| 6502  | [Supported drive modes](modules/state-machine/#supported-drive-modes)           |

## Module profile-torque

| Index | name                                                   |
| ----- | ------------------------------------------------------ |
| 6071  | [Target torque](modules/profile-torque/#target-torque) |
| 6087  | [Torque slope](modules/profile-torque/#torque-slope)   |

## Module torque-loop

| Index | name                                                              |
| ----- | ----------------------------------------------------------------- |
| 6077  | [Torque actual value](modules/torque-loop/#torque-actual-value)   |
| 6078  | [Current actual value](modules/torque-loop/#current-actual-value) |

## Module units

| Index | name                                                        |
| ----- | ----------------------------------------------------------- |
| 6091  | [Gear ratio](modules/units/#gear-ratio)                     |
| 6092  | [Feed constant](modules/units/#feed-constant)               |
| 60a8  | [SI unit Position](modules/units/#si-unit-position)         |
| 60a9  | [SI unit Velocity](modules/units/#si-unit-velocity)         |
| 60aa  | [SI unit Acceleration](modules/units/#si-unit-acceleration) |
| 60ab  | [SI unit Jerk](modules/units/#si-unit-jerk)                 |

## Module general

| Index | name                                              |
| ----- | ------------------------------------------------- |
| 67fe  | [Version number](modules/general/#version-number) |
