# Xreader packaging for Debian

This repository contains Debian packaging for [Linux Mint's Xreader](https://github.com/linuxmint/xreader). It targets upstream **4.6.9** as source version **4.6.9+ds-1**. The `+ds` repack excludes upstream's `debian/` directory, leaving this repository's packaging as the single source of truth.

The source builds `xreader`, `xreader-common`, `libxreaderdocument3`, `libxreaderview3`, their two `-dev` packages, and `gir1.2-xreader-1.5`. Installing `xreader` pulls in the common data and runtime libraries. The development and introspection packages are separate. Debhelper generates debug symbol packages automatically.

## Build on Debian unstable

Use a clean Debian unstable build VM with `devscripts`, `dpkg-dev`, and `build-essential`. In this packaging repository:

```sh
uscan --download-current-version
mkdir -p ../xreader-4.6.9+ds
tar -xf ../xreader_4.6.9+ds.orig.tar.xz -C ../xreader-4.6.9+ds --strip-components=1
cp -a debian ../xreader-4.6.9+ds/
cd ../xreader-4.6.9+ds
sudo apt build-dep .
dpkg-buildpackage -us -uc
lintian -i -I --pedantic ../xreader_4.6.9+ds-1_*.changes
```

The upstream Meson tests require a dogtail controlled graphical desktop; the build skips them. `autopkgtest` checks installed files, the PDF backend, the version and both introspection namespaces. In a fresh Debian Cinnamon VM, also open PDF, DjVu and XPS documents, check thumbnails and printing, and test both a Wayland and an X11 session if available.

## Salsa and mentors

The intended Salsa project is `https://salsa.debian.org/Overseer/xreader`. Create it and push the same packaging history there. Set the project's CI configuration file to `debian/salsa-ci.yml` in **Settings → CI/CD → General pipelines**.

An earlier Xreader ITP exists as [Debian bug #958981](https://bugs.debian.org/958981) under another prospective packager. Coordinate with that person and Debian's WNPP process before claiming the ITP or adding a `Closes:` entry. Once the package builds, installs, passes CI and copyright review, update `UNRELEASED` to `unstable`, build and sign a source package (`dpkg-buildpackage -S`), and upload it to mentors.debian.net for sponsor review. GitHub and Salsa do not upload a package to Debian automatically.
