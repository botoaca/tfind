# Torrent Finder
tfind is a small utility written in C to help quickly find torrents from trackers.

## Modularity
tfind is highly expandable and modular. to use tfind, you only have to build the trackers you want to use and the dependencies for that certain tracker. anyone can write an expansion based on the provided examples and implement it into the core program.

## Compatibility Progress
| Tracker   | Status |
|-----------|--------|
| Filelist  | ✅     |
| 1337x     | ✅     |
| rutracker | ✅     |

#### Legend:
| Symbol |      Status      |
|--------|------------------|
|   ✅   | Complete         |
|   ❔   | See observations |
|   ❌   | Incomplete       |

## Third party dependencies
for some components, json parsing is required. for that, the [parson](https://github.com/kgabis/parson) has been used.

## Building
to customize your tfind build, modify the given `build.sh` script
examples:
```
gcc main.c parson.c curl_callbacks.c credentials.c filelist.c -o tfind -lcurl (builds filelist only)
gcc main.c convert_encoding.c curl_callbacks.c credentials.c one337x.c rutracker.c -o tfind -lcurl -liconv (builds 1337x + rutracker)
gcc main.c curl_callbacks.c one337x.c -o tfind -lcurl (builds 1337x only)
```

## Observations
- in order to use the filelist component, a credentials file is needed. the `creds_filelist.txt` should look like this: `[username] [passkey]`.
- in order to use the rutracker component, a credentials file is needed. the `creds_rutracker.txt` should look like this: `[username] [password]`.
- while using the rutracker component, a `cookies_rutracker.txt` file will be created.