for ($n = 0;$n <= $#ARGV;$n++) {
	printsize($ARGV[$n]);
}

sub printsize() {
	local($filename) = @_;
	($dev,   $ino,   $mode, $nlink,
	 $uid,   $gid,   $rdev, $size, 
	 $atime, $mtime, $ctime,$blksize, 
	 $blocks) = stat($filename);
	$filename =~ s/^.+\\([^\\]+)$/$1/;
	
	print $filename."=>".$size."\n";
}
