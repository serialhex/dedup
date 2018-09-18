# Dedup - A File Deduplication Helper

This is a simple program to help you find duplicate files on your computer. It's pretty much beta-ware, but all the necessary elements are there, even if it is missing some nice features (like command-line arguments, and such).

## Why?

I have a NAS at home with __*LOTS*__ of storage space.  Which means the space gets used inefficiently sometimes, as people (like myself) copy items all over the place with no regard as to whether it is already located somewhere on the server or not.  So this little program is an aid to that process.

## How does it work?

Without getting too deep into the code itself, it basically does these things:

1. Initialize a tree.
2. Reads each file & directory in its current directory:
    * If it's a directory, recurse into the directory and repeat step 2
    * If it's a file, generate a hash code, get the file size, and stick it in the tree if there isn't anything with the same size and hash code already

And that's basically it.

Right now it doesn't have any way to interface with the executable.  You stick it in a directory, and either double-click it or run it in a console.  It will recursively run though the files and directories it can get to.  While it is running, it will write to a file called `dup-files.txt` in the same directory it is in.  And that's it.

## Can I use this on my production server running all my important buisness tasks?

You probably shouldn't, but feel free!  It's MIT-licensed, and writes to only one file, and reads all the rest, so your data __should__ be safe.  Though I won't guarantee it.  I personally am fine with it running on my home NAS with all my sons baby pictures and various media on it.

## How does it work, really

The main part of the algorithm is a simple 64bit Fibonacci hash function, like so:

~~~ C
const uint64_t Fib64 = 11400714819323198485llu;

inline hash_value fibonacci_hash(hash_value hash) {
  return hash * Fib64;
}
~~~

Being a simple multiplication hash makes it lightning fast (not much is faster than a single unsigned multiplication, maybe an XOR or add...) and effective, because we don't need it to be a *secure* hash function (like MD5 or SHA).  Also it calculates the size while it is calculating the hash, to reduce the total load by not seeking to the end of a file and then calculating the hash and then going back to the beginning of the file... you get the idea.  It does that thusly:

~~~ C
size_t bytes_read = 0;
bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, data);
while (bytes_read) {
  fhash.filesize += bytes_read;
  for(size_t i = 0; i < bytes_read; i++) {
    fhash.hash = fibonacci_hash(fhash.hash + buffer[i]);
  }
  bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, data);
}
~~~

Where `BUFFER_SIZE` is a size in bytes to use.  It's currently set at 16MB (as I have a number of large files, and I'd rather read as much as possible as fast as possible, but may be optimized).

## Improvements & the Future

### Binary Tree

Right now I use a (very) simple binary tree structure to keep track of what values have already been calculated.  It is an unbalanced binary tree, so there is the chance that one side may become much deeper than the other, changing it from O(log n) to O(n) search & insertion time for some values.

I will probably end up implementing something like an RB-Tree in the future, mostly to alleviate the unbalanced tree problem, and because I've never implemented an RB-Tree before (I've never implemented *any* binary tree before this, so lots of firsts here!)

Also, I used a binary tree instead of an array-based hash table because the number of files on my server is well over several thousand or so.  Lots of little files, pictures, text files, git repos, etc...  I didn't want to deal with re-instantiating a whole table once it got past however many items.  Also, since I'd be dropping bits from the hash, it would increase the chance for a collision even if two files were unequal.

### Command Line Arguments

Right now, the utility doesn't accept any command line arguments.  Not even a directory to start from.  This is mostly because I've been too lazy to implement it right now, and I don't need it.  Sometime in the near future I will probably implement this.

### Multithreading

Right now this is very single-threaded.  Which isn't really a bad thing.  It has saturated the network between one of my laptops and my NAS, and is only using about 16-20% of the quad-core i3 processor on that laptop.  The gigabit ethernet is pulling ~300Mbps of data when it's big files, so there probably isn't any need to do this...

But this is a learning project, right?  So if I get around to it, I'll see about implementing semaphores in the tree so changing it doesn't hurt much, and then probably have a thread pool with one thread per directory that runs until completion... Or something like that.
