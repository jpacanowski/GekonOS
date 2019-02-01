rm -rf *.o
rm -rf *.bin
rm -rf *.iso
rm -rf libc.a

read -p "Commit name: " commit

git add .
git commit -m "$commit"
git push -u origin master
