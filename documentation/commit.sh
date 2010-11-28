#!/bin/bash

BUILDDIR=_build

git clone -b gh-pages git@github.com:nullable/libxmlquery.git /tmp/gh_pages
cp -r $BUILDDIR/html/* /tmp/gh_pages/
pushd /tmp/gh_pages
git add .
git commit -m 'Auto-commit by make gh_pages target'
git push origin gh-pages:gh-pages
popd
rm -rf /tmp/gh_pages