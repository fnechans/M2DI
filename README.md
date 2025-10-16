# M2DI


## install

Requires `meson` and `ninja`, ideally install in a virtualenv

```bash
python3 -m venv env
source env/bin/activate
pip install meson ninja
```

Then sdl needs to be installed

```bash
meson wrap install sdl2
meson wrap install sdl2_image
meson wrap install sdl2_ttf
```

On ubuntu, readline lib is needed

```bash
sudo apt-get install libreadline-dev
```

Finally build the project

```bash
meson setup builddir
meson compile -C builddir
```