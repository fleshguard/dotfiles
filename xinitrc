#Dual Monitor Config
xrandr --output HDMI2 --primary --left-of eDP1 --brightness 0.70 && xrandr --output eDP1 --brightness 0.75
xwallpaper --output HDMI2 --zoom /home/wallpapers/rand_wallpaper_14.jpg && xwallpaper --output eDP1 --zoom /home/wallpapers/rand_wallpaper_14.jpg

slstatus &
picom -b

exec dwm
