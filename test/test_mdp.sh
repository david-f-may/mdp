#!/bin/bash

################################################################################
# You should run this as root, as it will traverse through /usr and grab
# regular files.
################################################################################

# fdp->path,
# fdp->mode,
# fdp->owner,
# fdp->group,
# (long long) fdp->size,
# fdp->dt,
# fdp->crc,
# fdp->md5,
# fdp->sha,
# fdp->sha256,
# fdp->sha512,
# fdp->rmd);
 
#find /home/dfmay -type f > mdp_test_file.txt
find /usr -type f |grep -v /usr/share > mdp_test_file.txt
for f in `cat mdp_test_file.txt`; do
	/bin/echo "$f ..." | tee -a mdp_test_output.txt
	# Get data needed for comparison
	s_me=`ls -l $f | tail -1 | awk '{print $1}'`
	s_ow=`ls -l $f | tail -1 | awk '{print $3}'`
	s_gp=`ls -l $f | tail -1 | awk '{print $4}'`
	s_sz=`ls -l $f | tail -1 | awk '{print $5}'`
	if [ "$s_sz" -eq "0" ]; then continue; fi
	s_dt=`stat --print="%z" $f | cut -f1 -d'.'`
	s_cr=`crc64sum $f | awk '{print $1}'`
	s_md=`md5sum $f | awk '{print $1}'`
	s_sh=`sha1sum $f | awk '{print $1}'`
	s_s2=`sha256sum $f | awk '{print $1}'`
	s_s5=`sha512sum $f | awk '{print $1}'`
	s_rm=`openssl rmd160 $f | awk '{print $2}'`

	# Now, get the data from mdp.
	m_me=`mdp -qc " md ( \"$f\", \"mode\"); " | cut -f2 -d'|'`
	m_ow=`mdp -qc " md ( \"$f\", \"owner\"); " | cut -f2 -d'|'`
	m_gp=`mdp -qc " md ( \"$f\", \"group\"); " | cut -f2 -d'|'`
	m_sz=`mdp -qc " md ( \"$f\", \"size\"); " | cut -f2 -d'|'`
	m_dt=`mdp -qc " md ( \"$f\", \"dt\"); " | cut -f2 -d'|'`
	val=`python -c "print (\"$m_dt\".replace('-',' '))"`
	m_dt=`python -c "print (\"$val\".replace('/','-'))"`
	m_cr=`mdp -qc " md ( \"$f\", \"crc\"); " | cut -f2 -d'|'`
	m_md=`mdp -qc " md ( \"$f\", \"md5\"); " | cut -f2 -d'|'`
	m_sh=`mdp -qc " md ( \"$f\", \"sha\"); " | cut -f2 -d'|'`
	m_s2=`mdp -qc " md ( \"$f\", \"sha256\"); " | cut -f2 -d'|'`
	m_s5=`mdp -qc " md ( \"$f\", \"sha512\"); " | cut -f2 -d'|'`
	m_rm=`mdp -qc " md ( \"$f\", \"rmd\"); " | cut -f2 -d'|'`

	# Okay, compare them.
	if [ "$s_me" != "$m_me" ]; then echo "***ERROR=mode:   $s_me != $m_me" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_ow" != "$m_ow" ]; then echo "***ERROR=owner:  $s_ow != $m_ow" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_gp" != "$m_gp" ]; then echo "***ERROR=group:  $s_gp != $m_gp" | tee -a mdp_test_output.txt; echo ""; fi
	if [ $s_sz -ne $m_sz ];    then echo "***ERROR=size:   $s_sz != $m_sz" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_dt" != "$m_dt" ]; then echo "***ERROR=dt:     $s_dt != $m_dt" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_cr" != "$m_cr" ]; then echo "***ERROR=crc64: $s_cr != $m_cr" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_md" != "$m_md" ]; then echo "***ERROR=md5:    $s_md != $m_md" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_sh" != "$m_sh" ]; then echo "***ERROR=sha:    $s_sh != $m_sh" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_s2" != "$m_s2" ]; then echo "***ERROR=sha256: $s_s2 != $m_s2" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_s5" != "$m_s5" ]; then echo "***ERROR=sha512: $s_s5 != $m_s5" | tee -a mdp_test_output.txt; echo ""; fi
	if [ "$s_rm" != "$m_rm" ]; then echo "***ERROR=rmd:    $s_rm != $m_rm" | tee -a mdp_test_output.txt; echo ""; fi
	sleep .25
done
rm -f mdp_test_file.txt
