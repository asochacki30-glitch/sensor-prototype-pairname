# The Doorman

## It sees
How near you are, in cm.

- Nobody: 400.0 cm, empty space and no object in range.
- A little: 75.0 cm, someone or something crossing the path.
- A lot: 28.5 cm, a hand or body very close to the sensor.

## It calls you
- `intruder`: under 30 cm, held 1 s
- `hesitant`: under 80 cm, held 4 s
- `nobody`: over 80 cm, held 5 s

## It does
- `intruder`: red LED blinks and the OLED says `INTRUDER`
- `hesitant`: yellow LED glows and the OLED says `HESITANT`
- `nobody`: all lights off and the OLED says `OPEN`

## For them
Intruder: it warns you off the moment you stop being just a passing shape and stay close too long.

## To them
Intruder: it calls closeness a threat.

## It refuses
It will not count your visits or explain why it named you that way. It only shows the final judgment.

## Set aside
Leo's Waiter: no stand by 11/18.

## Sits next to
Hansel and Gretel, and pushes back.

## The tension
Leaning in to read the word makes you an intruder.

## State Table
| Name | Rule | LINE | GAP | Hold | Visitor learns it by | Piece does | For them / To them |
|---|---|---:|---:|---:|---|---|---|
| intruder | under 30 cm | 30 | 3 | 1 s | word card under red LED | D5: red blink | For them: it warns you off. To them: it calls closeness a threat. |
| hesitant | under 80 cm | 80 | 5 | 4 s | word card under yellow LED | D6: yellow on | For them: it waits. To them: it calls a pause doubt. |
| nobody | over 80 cm | 80 | 5 | 5 s | empty screen, open state | all dark | For them: the space is open. To them: it leaves you alone. |

The row order matters. The first true condition with its hold wins, so the narrow name sits above the wider one.
