# VisualC++ CL.EXE�� JAVA�����ɤΥץ�ץ����Ѥ˸Ƥӵ���
#
# 1- �ץ�ץ�����������
# ��ư: javapp.pl <CppFilePath> <JavaFilePath>
# �ץ�ץ��������򤷤�  <FileName>.java �Ȥ����ե�������������ޤ�
# ���ե�����̾/�Ԥ��б�ɽ <FileName>.line �Ȥ����ե�������������ޤ�
#
# 2- Ant��javac���Ϸ�̤Υ��顼�ԡ����顼�ե�����̾�򸵤�CPP�ե�����̾���Ѵ�����
#     ��ư: javapp.pl
#     ɸ�����Ϥ���ANT��javac���Ϸ�̤��ɤ߼��ޤ�
#     ���顼�����ȹ礻 <FileName>.java,<LineNo> �� ����ɽ���Ǽ������ޤ�
#     <FileName>.line���ɤ߹��ߡ�(<FileName>.java,<LineNo>)��(<FileName>.cpp,<LineNo>)���Ѵ����ޤ�
#     �Ѵ�������̤���Ϥ��ޤ�
#


if ($ARGV[0] eq "pp") {
	&pp;
} else {
	&antfilter;
}

sub pp() {
	print "JavaPP\n";
	print "copyright 2002 spicysoft\n";

	$cppfilepath  = $ARGV[1];
	$javafilepath = $ARGV[2];
	$ppcmd        = $ARGV[3]." ".$ARGV[4]." ".$ARGV[5]." ".$ARGV[6]." ".$ARGV[7];
	$linefilepath = $javafilepath;
	$linefilepath =~ s/(.+)\.java/$1.line/;
	print "cppfilepath=$cppfilepath\njavafilepath=$javafilepath\nlinefilepath=$linefilepath\n$ppcmd\n";
	
	$javaline = 0;
	open CPP,"cl.exe /E /nologo $ppcmd $cppfilepath |" or die"�ץ�ץ����������Ǥ��ʤ��ä�";
	open JAVA,">".$javafilepath or die "�Ѵ���̤�JAVA�����������ɤ���¸�Ǥ��ʤ�";
	open LINE,">".$linefilepath or die "�Ծ���ե����뤬�����ʤ�";
	
	$cppline  = 0;
	$cppfname = $cppfilepath;
	while (<CPP>) {
		$javaline++;
		if (/^\s*\#line\s+(\d+)\s+\"(.+)\"$/) {
			$cppline  = $1-1;
			$cppfname = $2;
			print (JAVA "\n");
		} else {
			$cppline++;
			print (JAVA $_);
		}
		print (LINE $cppline.",".$cppfname."\n");
	}
	
	close(CPP);
	close(JAVA);
	close(LINE);
}


sub antfilter() {
	my($indent);
	my($linefilepath);
	my($javaerrline);
	my($javaerrdesc);
	
	$filepath = $ARGV[0];
	$linedir  = $ARGV[1];
	
	open FILE,$filepath or "�ե����뤬�ɤ߹���ʤ�";
	
	while (<FILE>) {
		if (/^(\s+\[junit\]\s+at\s+[\w\.]+)\((\w+)\.java\:(\d+)\)$/) {
			$indent       = $1;
			$linefilepath = $linedir . "\\" . $2.".line";
			$javaerrline  = $3;
			&get_line($linefilepath,$2.".java",$javaerrline);
			print $indent."(".$cpppath.":".$lineno.")\n";

		} elsif (/^(\s*)\[javac\]\s*(.+)\.java\:(\d+)\:(.+)$/) {
			$indent       = $1;
			$linefilepath = $2.".line";
			$javaerrline  = $3;
			$javaerrdesc  = $4;
			&get_line($linefilepath,$2.".java",$javaerrline);
			print $indent."[javac] ".$cpppath.":".$lineno.":".$javaerrdesc."\n";

		} else {
			print $_;
		}
	}
	
	close(FILE);
}


sub get_line() {
	local($linefilepath,$javapath,$javaerrline) = @_;

	my($line);
	my($lines);
	
	open(LINE,$linefilepath);
	@lines = <LINE>;
	close(LINE);

	$line = $lines[$javaerrline-1];
	if ($line =~ /^(\d+)\,(.+)$/) {
		$cpppath = $2;
		$lineno  = $1;
	} else {
		$cppath  = $javapath;
		$lineno  = $javaerrline;
	}
}
