# Sensor prototype: ultrasonic presence detector

This build uses an HC-SR04 distance sensor and a browser response. The Nano reads a real distance in centimeters, `bench/sense-serial.js` converts it to a 0-to-1 signal, and `bench/table-bench.js` decides whether the space is empty or occupied.

## Sensor and wiring

The sensor is an HC-SR04 ultrasonic distance module. It is the simplest shelf sensor that gives a numbered reading and a relevant response in a single minute.

| Part | Nano pin | Sensor pin |
|---|---|---|
| 5V | 5V | VCC |
| GND | GND | GND |
| Trigger | D9 | TRIG |
| Echo | D10 | ECHO |

The sketch in `bench/bench.ino` triggers the sensor with a 10 µs pulse, measures the echo time, and prints `cm` values at 115200 baud. When nothing is in range, it reports `400.0 cm` so the browser can treat that as `open` instead of a broken connection.

## The rule

Three real readings from the sensor:

- 400.0 cm: open path / nothing in front
- 75.0 cm: a hand or body entering the zone
- 28.5 cm: a near blockage right in front of the sensor

State Table sentence:

When the reading drops below 60 cm, the Nano marks the path as blocked and changes the response; when it rises above 85 cm, the lane is open again.

This is the logic in `bench/table-bench.js`:

- `blocked` when `near > 0.82` for 0.5 s
- `watching` when `present > 0.6` for 0.9 s
- `open` when `present < 0.2` for 1.5 s

That produces a distinct response while staying stable enough not to flicker when the echo wobbles at the threshold.

Final verification: a hand at about 30 cm reads as `blocked`, a person at 75 cm reads as `watching`, and an empty space at 400 cm remains `open`.

## Output

The response is visible in the browser using the same bench piece as the starter project. The color and size of the piece change based on the current state, so a visitor can tell immediately whether the sensor sees a blocked path or an open space.

This makes the machine notice one real thing about the world: whether a path in front of it is being crossed or left empty.

## Break log

| Date | Commit hash | What broke | What changed |
|---|---|---|---|
| 2026-10-26 | `a92d31f` | The trigger and echo lines were not matching the test bench, so the reading stuck at 400 cm. | Rewired the HC-SR04 to D9 and D10 and verified the pulse timing in the sketch. |
| 2026-10-26 | `b7ff4c1` | The state toggled at the threshold and flickered as a hand moved across the line. | Increased the hold time on the close and here rows to smooth the signal. |
| 2026-10-26 | `e8dc62a` | The browser state names did not match the meaning of the sensor, so the response was unclear. | Renamed the outputs to the more readable close/here/nobody pattern and documented the rule. |

## Credits

- Adam Sochacki — wiring and serial reading link
- Gabriel Everett — rule and sensor threshold tuning link
- Marco Cericola — response and documentation link

Each person owned one end-to-end link: reading the sensor, deciding the rule, or making the visible response.
