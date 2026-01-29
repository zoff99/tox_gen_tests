#! /bin/bash

repo_url="https://github.com/TokTok/c-toxcore"
repo_name="c-toxcore"
repo_commit="master"

rundir="_run"

_HOME2_=$(dirname "$0")
export _HOME2_
_HOME_=$(cd "$_HOME2_" || exit;pwd)
export _HOME_

cd "$_HOME_"
pwd

# sudo apt -y install libtool autoconf libsodium-dev

mkdir -p "$rundir"/
cd "$rundir"/ || exit 1

git clone "$repo_url"
cd "$repo_name""/"

git checkout "$repo_commit"

rm -Rf third_party/cmp
git submodule update --init --recursive

./autogen.sh
./configure --disable-av || exit 1

make -j10 || exit 1


ls -al build/.libs/libtox*.a || exit 1


cd "$_HOME_"
cd tests/ || exit 1

INCDIR="$_HOME_"/"$rundir"/c-toxcore/toxcore/
LIBDIR="$_HOME_"/"$rundir"/c-toxcore/build/.libs/

for i in $(ls -1 *.c) ; do
    echo "$i"
    gcc -g -O0 -fsanitize=address "$i" -I"$INCDIR" "$LIBDIR"/libtoxcore.a "$LIBDIR"/libtoxencryptsave.a -fPIC -Wl,-Bstatic $(pkg-config --cflags --libs libsodium) -Wl,-Bdynamic -pthread -o "${i%.c}" && ./"${i%.c}"
done



