# Xreader packaging for Debian

This repository contains Debian packaging for [Linux Mint's xreader](https://github.com/linuxmint/xreader), targeting upstream tag **4.6.9** and source version **4.6.9-1**. The inspected tag points to commit `a4e5ed2c916e6241efb7d9e86d01b709e45a18d8`. Upstream code is downloaded separately.

The source builds `xreader`, `xreader-common`, `libxreaderdocument3`, `libxreaderview3`, their two `-dev` packages, and `gir1.2-xreader-1.5`. Installing `xreader` pulls in data and runtime libraries. The combined introspection package contains the typelibs; the development packages contain their corresponding GIR XML. Debhelper generates debug symbol packages automatically.

## Build and check

Use a minimal Debian unstable build VM and a separate Debian Cinnamon desktop VM for interactive tests. Install `build-essential`, `devscripts`, `dpkg-dev`, `lintian`, `sbuild`, and `autopkgtest` on the build VM. Configure an unstable sbuild/schroot testbed before using the isolated commands below.

From this packaging checkout:

```sh
uscan --download-current-version --destdir ..
mkdir ../xreader-4.6.9
tar -xf ../xreader_4.6.9.orig.tar.gz -C ../xreader-4.6.9 --strip-components=1
# Replace the extracted Linux Mint packaging completely.
rm -rf ../xreader-4.6.9/debian
cp -a debian ../xreader-4.6.9/
cd ../xreader-4.6.9
sudo apt build-dep .
dpkg-buildpackage -us -uc
lintian -i -I --pedantic ../xreader_4.6.9-1_*.changes
sbuild -d unstable ../xreader_4.6.9-1.dsc
autopkgtest ../xreader_4.6.9-1_amd64.changes -- schroot unstable-amd64-sbuild
```

The `mkdir` deliberately fails if the build tree already exists; start with a fresh tree for each source preparation. Replace `amd64` and the testbed name with your configured architecture and schroot.

Use the original upstream archive, without a `+ds` repack just to remove `debian/`. Source format `3.0 (quilt)` replaces that directory when extracting the Debian source package. This packaging-only repository does not assume imported upstream or pristine-tar branches. `debian/watch` discovers numbered upstream tags; inspect and update the changelog before packaging a newer release.

The runtime test generates a one-page PDF, renders a thumbnail and loads the document through introspection. The development test compiles and links against both public libraries, loads that PDF and creates a view. Upstream dogtail tests need an interactive desktop and are skipped during the package build.

Install the resulting runtime packages with APT on the separate Cinnamon VM. Test opening, saving where applicable, help, printing, thumbnails and plugins, including Wayland and X11 sessions where available. Automated smoke checks do not cover all interactive behavior.

## Salsa and submission

The intended Salsa project is `https://salsa.debian.org/Overseer/xreader`. Push the packaging history there and set the CI configuration path to `debian/salsa-ci.yml` under **Settings → CI/CD → General pipelines**. The standard Salsa recipe is retained.

An earlier Xreader ITP exists as [#958981](https://bugs.debian.org/958981). Coordinate with its prospective packager before claiming it or adding a `Closes:` entry.

Keep the changelog `UNRELEASED` during preparation. After clean Debian unstable builds, installed-package tests, desktop checks and copyright review pass, finalize it for `unstable`, build and sign a source upload on the machine holding your signing key, and upload it to mentors.debian.net for sponsor review. GitHub commits and Salsa CI do not upload to Debian. Do not commit binaries; any test binary release should include the matching source, `.changes`, `.buildinfo` and checksums.

Update the `Vcs-*` fields to the exact Salsa URL after making that project canonical.
