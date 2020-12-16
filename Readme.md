# AsMatch

## Description
A simple tool for learning Classless Inter-Domain Routing (CIDR). This tool maps an IP address to an AS via longest prefix matching with a static table consisting of whois and BGP data.

## Usage
`asmatch <file with static DB data> <file with list of IPs>`
 
## IP Format
A list of newline-separated IPv4 addresses.

    148.30.33.60  
    182.129.218.12  
    ...

## DB format
A list of newline separated entries corresponding an AS with its subnet address and CIDR suffix:

    12.110.70.0 23 5650
    61.93.224.0 24 9269
    154.17.132.0 24 11908
    202.183.168.0 21 7568
    ...
  
## Output
The output will show the determined correspondences.

### Example
#### IP list

    169.237.33.90
    208.30.172.70

#### DB

    169.237.0.0 16 1852
    208.0.0.0 11 1239

#### Output

    169.237.0.0/16  1852 169.237.33.90
    208.0.0.0/11  1239 208.30.172.70

## Compile
Run cmake in the current directory:

`$ cmake .`

Then `make` using the generated Makefile:

`$ make`  

You're done! The asmatch executable is now generated in the project root.

