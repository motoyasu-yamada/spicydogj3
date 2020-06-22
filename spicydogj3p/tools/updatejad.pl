$jadpath = $ARGV[0];
$jarpath = $ARGV[1];

local($filename) = @_;
	($dev,   $ino,   $mode, $nlink,
	 $uid,   $gid,   $rdev, $size, 
	 $atime, $mtime, $ctime,$blksize, 
	 $blocks) = stat($jarpath);


open JAD,$jadpath;
while (<JAD>) {
	s/\$\{JarSize\}/$size/;
	$jad .= $_;
}
close JAD;

open JAD,"> $jadpath";
print JAD $jad;
close JAD;

