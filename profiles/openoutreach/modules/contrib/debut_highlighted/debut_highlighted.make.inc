; Drupal version
core = 7.x
api = 2

; Contrib modules
projects[advanced_help][subdir] = contrib
projects[advanced_help][version] = 1.0
projects[apps_compatible][subdir] = contrib
projects[apps_compatible][version] = 1.0-alpha3
projects[context][subdir] = contrib
projects[context][version] = 3.0-beta4
projects[ctools][subdir] = contrib
projects[ctools][version] = 1.2
projects[debut_highlighted][subdir] = contrib
projects[debut_highlighted][version] = 1.0-rc3
projects[features][subdir] = contrib
projects[features][version] = 1.0
projects[file_entity][subdir] = contrib
projects[file_entity][version] = 2.0-unstable7
projects[media][subdir] = contrib
projects[media][version] = 2.0-unstable7
projects[media_vimeo][subdir] = contrib
projects[media_vimeo][version] = 1.0-beta5
projects[media_youtube][subdir] = contrib
projects[media_youtube][version] = 1.0-beta3
projects[pathauto][subdir] = contrib
projects[pathauto][version] = 1.2
projects[strongarm][subdir] = contrib
projects[strongarm][version] = 2.0
projects[token][subdir] = contrib
projects[token][version] = 1.4
projects[views][subdir] = contrib
projects[views][version] = 3.5
projects[views_slideshow][subdir] = contrib
projects[views_slideshow][version] = 3.0

libraries[jquery.cycle][download][type] = "get"
; Permissions wrong when specifying single file. See http://drupal.org/node/1459618.
; libraries[jquery.cycle][download][url] = "https://raw.github.com/malsup/cycle/master/jquery.cycle.all.js"
libraries[jquery.cycle][download][url] = "https://github.com/malsup/cycle/tarball/master"
; Optional so disabled for now pending a fix to http://drupal.org/node/1459618.
; libraries[json2][download][type] = "get"
; libraries[json2][download][url] = "https://github.com/douglascrockford/JSON-js/raw/master/json2.js"
