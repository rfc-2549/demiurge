# Demiurge

Attempt of client of Pleroma (It may work with mastodon as well, but
haven\'t tried) written in C because i\'m criminally insane

## How to use

Demiurge highly uses the UNIX philosophy

``` shell
./demiurge -s "Hello this is a status" -v "unlisted" # Post "Hello
# this is a status" with the unlisted schope

./demiurge -F "filename.jpg" -s "ebin maymay :DDD" # Post "ebin
# maymay" with "filename.jpg" attatched, the file description will be
# the same as the filename.

for file in *.jpg; do ./demiurge -F $file -s "sbammin :DD"; done # Posts
# all jpgs files in different statuses
cat /etc/passwd | ./demiurge # Posts the output of /etc/passwd
```

demiurge is a tool to make fedi bots using shell scripts rather than a
client you\'d use. For example:

``` shell
export x=$(curl https://some-website.tld)
./demiurge -s "current weather: $x"
```

## TODO:

Check timeline and notifications, replies...

## THANKS TO

Dendy, who wrote the [Code
reference](https://git.fai.su/dendy/fedibooru)
