# InterLoop
InterLoop : Light-weight tool for inter-loop prediction with ChIA-PET or HiChIP datasets. 

---
## Requirements & Installation

### Environment Requirement
```markdown
gcc >= 8.3.0
cmake >= 3.20
```

### Installation
```shell
git clone https://github.com/devil-cry/InterLoop.git
cd InterLoop
cmake .
make
```

---
## Usage

### Help Message
```text
Optional arguments:
  -h, --help                 	shows help message and exits 
  -v, --version              	prints version information and exits 
  -f, --file                 	filename of valid pairs in '.bedpe' format. [required]
  -o, --output               	prefix of output loop filename. [required]
  -t, --thread               	threads used to run InterLoop. [default: 1]
  -ws, --window_size         	anchors that are within the sliding window size from one bedpe to another will be considered relevant. [default: 10000]
  -inc_chr, --include_chromes	only process the anchor whose one of the end is contained on the specified chromatin, used ',' to split. [default: "all"]
  -inc_pair, --include_pairs 	only process the anchor whose both ends are contained on the specified chromatin, each pair in 'chrA-chrB' format, and used ',' to split. [default: "all"]
  -lo, --log_output          	log file name [default: "InterLoop.log"]
  -lv, --log_level           	level of logger, select in {ERROR,WARN,INFO,DEBUG}. [default: "INFO"]
```
### Example of usage 
```shell
./InterLoop -f ./res/dataset_example_valid.bedpe -o example -t 3 -ws 10000 -lv DEBUG -lo example.log -inc_pair chr1-chr2,chr6-chr7,chr5-chr6
```

---
## Additional function
Under development.

## Output
Output inter-loops in 'chrA startA endA chrB startB endB' format in {chrA-chrB}.loop .