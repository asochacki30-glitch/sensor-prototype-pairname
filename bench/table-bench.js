// bench/table-bench.js: the State Table for bench pairs, in code.
//
// bench.html loads this file instead of table.js, so a bench pair's rows live
// here. Same format as table.js: signals are the numbers sense-serial.js writes
// (each 0 to 1, named in its SIGNALS), states are checked top to bottom and the
// first true row wins.
//
// hold is in seconds: how long a rule must stay true before the state changes.
// It is a tradeoff you choose per row. A short hold feels instant but flickers
// when a reading wobbles near its line; a long hold is calm but slow. These
// example rows use 0.5, 1 and 2, the same as the working reference build.
// Keep "nobody" last, so it catches everything the other rows miss.

window.TABLE = {
  // The sensor notices whether the path in front of it is blocked or open.
  // A hand, body, or object within about 60 cm counts as "blocked"; farther away
  // than roughly 85 cm is treated as an open lane.
  signals: ['present', 'near'],

  states: [
    { name: 'blocked', when: s => s.near > 0.82,   hold: 0.5 },
    { name: 'watching', when: s => s.present > 0.6, hold: 0.9 },
    { name: 'open', when: s => s.present < 0.2,   hold: 1.5 },
  ],
};
