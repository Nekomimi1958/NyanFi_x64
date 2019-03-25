//----------------------------------------------------------------------//
//�R�}���h�ꗗ															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "usr_file_ex.h"
#include "usr_tag.h"
#include "usr_cmdlist.h"

//---------------------------------------------------------------------------
UnicodeString ScrModeIdStr = "FSVIL";

//---------------------------------------------------------------------------
//�R�}���h�ꗗ���X�g��ݒ�
//---------------------------------------------------------------------------
void set_CmdList(
	TStringList *c_list,	//[o] �R�}���h��=����
	TStringList *s_list)	//[o] ���ʕ���:�R�}���h��=����
{
	c_list->Text =
		"F:AboutNyanFi=�o�[�W�������\n"
		"F:AddTab=�^�u��ǉ�\n"
		"F:BackDirHist=�f�B���N�g��������߂�\n"
		"F:Backup=���΃p�X�Ƀo�b�N�A�b�v\n"
		"F:BgImgMode=�w�i�摜�̕\�����[�h�ݒ�\n"
		"F:BorderLeft=�t�@�C�����X�g�̋��E�����Ɉړ�\n"
		"F:BorderRight=�t�@�C�����X�g�̋��E���E�Ɉړ�\n"
		"F:CalcDirSize=�f�B���N�g���e�ʂ��v�Z\n"
		"F:CalcDirSizeAll=�S�f�B���N�g���̗e�ʂ��v�Z\n"
		"F:ChangeDir=�f�B���N�g����ύX\n"
		"F:ChangeDrive=�h���C�u�̕ύX\n"
		"F:ChangeOppDir=���Α��̃f�B���N�g����ύX\n"
		"F:ChangeOppRegDir=���Α��œo�^�f�B���N�g�����J��\n"
		"F:ChangeRegDir=�o�^�f�B���N�g�����J��\n"
		"F:CheckUpdate=�X�V�̊m�F\n"
		"F:ClearMask=�I���}�X�N/�p�X�}�X�N������\n"
		"F:Clone=���΃p�X�ɃN���[���쐬\n"
		"F:CloneToCurr=�J�����g�ɃN���[���쐬\n"
		"F:CommandPrompt=�R�}���h�v�����v�g\n"
		"F:CompareDlg=�����t�@�C���̔�r�_�C�A���O\n"
		"F:CompareHash=�n�b�V���l�̔�r\n"
		"F:CompleteDelete=���S�폜\n"
		"F:CompressDir=�f�B���N�g����NTFS���k\n"
		"F:ContextMenu=�R���e�L�X�g���j���[��\��\n"
		"F:ConvertDoc2Txt=�o�C�i���������e�L�X�g�ϊ�\n"
		"F:ConvertHtm2Txt=HTML���e�L�X�g�ϊ�\n"
		"F:ConvertImage=�摜�t�@�C���̕ϊ�\n"
		"F:ConvertTextEnc=�����R�[�h�̕ϊ�\n"
		"F:Copy=�R�s�[\n"
		"F:CopyCmdName=�R�}���h�����N���b�v�{�[�h�ɃR�s�[\n"
		"F:CopyDir=�f�B���N�g���\���̃R�s�[\n"
		"F:CopyFileName=�t�@�C�������N���b�v�{�[�h�ɃR�s�[\n"
		"F:CopyTo=�w��f�B���N�g���ɃR�s�[\n"
		"F:CopyToClip=�N���b�v�{�[�h�ɃR�s�[\n"
		"F:CountLines=�e�L�X�g�t�@�C���̍s�����J�E���g\n"
		"F:CreateDir=�f�B���N�g���̍쐬\n"
		"F:CreateDirsDlg=�f�B���N�g���ꊇ�쐬�_�C�A���O\n"
		"F:CreateHardLink=�n�[�h�����N�̍쐬\n" 
		"F:CreateJunction=�W�����N�V�����̍쐬\n" 
		"F:CreateShortcut=�V���[�g�J�b�g�̍쐬\n"
		"F:CreateSymLink=�V���{���b�N�����N�̍쐬\n" 
		"F:CreateTestFile=�e�X�g�t�@�C���̍쐬\n"
		"F:CsrDirToOpp=�J�[�\���ʒu�̃f�B���N�g���𔽑Α��ɊJ��\n"
		"F:CurrFromOpp=�J�����g�p�X�𔽑΃p�X��\n"
		"F:CurrToOpp=���΃p�X���J�����g�p�X��\n"
		"F:CursorEnd=�J�[�\�����ŉ��s�Ɉړ�\n"
		"F:CursorEndSel=�I�����Ȃ���J�[�\�����ŉ��s�Ɉړ�\n"
		"F:CursorTop=�J�[�\�����ŏ�s�Ɉړ�\n"
		"F:CursorTopSel=�I�����Ȃ���J�[�\�����ŏ�s�Ɉړ�\n"
		"F:CutToClip=�N���b�v�{�[�h�ɐ؂���\n"
		"F:DateSelect=�w�肵�����t�����ɍ����t�@�C����I��\n"
		"F:DeleteADS=�t�@�C���̑�փf�[�^�X�g���[�����폜\n"
		"F:DelJpgExif=Exif���̍폜\n"
		"F:DelSelMask=�I���}�X�N���獀�ڂ��폜\n"
		"F:DelTab=�^�u���폜\n"
		"F:DiffDir=�f�B���N�g���̔�r\n"
		"F:DirHistory=�f�B���N�g������\n"
		"F:DirStack=�f�B���N�g���E�X�^�b�N\n"
		"F:DistributionDlg=�U�蕪���_�C�A���O\n"
		"F:DotNyanDlg=.nyanfi �t�@�C���̐ݒ�\n"
		"F:DriveList=�h���C�u�ꗗ\n"
		"F:DriveGraph=�h���C�u�g�p������\n"
		"F:EditIniFile=INI�t�@�C���̕ҏW\n"
		"F:Eject=CD/DVD�h���C�u�̃g���C���J��\n"
		"F:EjectDrive=�h���C�u�̎��O��\n"
		"F:EmptyTrash=���ݔ�����ɂ���\n"
		"F:EqualListWidth=���E�̃��X�g�����ϓ���\n"
		"F:ExeCommandLine=�R�}���h���C���̎��s\n"
		"F:ExeExtMenu=�ǉ����j���[�̎��s\n"
		"F:ExeExtTool=�O���c�[���̎��s\n"
		"F:Exit=NyanFi�̏I��\n"
		"F:ExitDupl=��d�N�����ꂽNyanFi���I��\n"
		"F:ExPopupMenu=�g���|�b�v�A�b�v���j���[\n"
		"F:ExtractChmSrc=CHM����\�[�X�𒊏o\n"
		"F:ExtractGifBmp=�A�j��GIF����r�b�g�}�b�v�𒊏o\n"
		"F:ExtractIcon=�A�C�R���𒊏o\n"
		"F:ExtractImage=���ߍ��݉摜�𒊏o\n"
		"F:ExtractMp3Img=MP3�̖��ߍ��݉摜�𒊏o\n"
		"F:FileExtList=�g���q�ʈꗗ\n"
		"F:FileListOnly=�t�@�C�����X�g�݂̂�\��\n"
		"F:FileRun=�t�@�C�������w�肵�Ď��s\n"
		"F:Filter=�t�@�C�����X�g�̍i�荞��\n"
		"F:FindDuplDlg=�d���t�@�C���̌����_�C�A���O\n"
		"F:FindDirDlg=�f�B���N�g���������_�C�A���O\n"
		"F:FindFileDirDlg=�t�@�C��/�f�B���N�g���������_�C�A���O\n"
		"F:FindFileDlg=�t�@�C���������_�C�A���O\n"
		"F:FindFolderIcon=�t�H���_�A�C�R������\n"
		"F:FindHardLink=�n�[�h�����N���\n"
		"F:FindMark=�x�}�[�N���ڂ�����\n"
		"F:FindTag=�^�O����\n"
		"F:FixTabPath=�^�u�ύX�ɑ΂��ăJ�����g�p�X���ꎞ�Œ�/����\n"
		"F:FTPChmod=�p�[�~�b�V�����̐ݒ�\n"
		"F:FTPConnect=FTP�z�X�g�ɐڑ�\n"
		"F:FTPDisconnect=FTP�z�X�g����ؒf\n"
		"F:ForwardDirHist=�f�B���N�g��������i��\n"
		"F:GetHash=�t�@�C���̃n�b�V���l���擾\n"
		"F:GitDiff=�J�[�\���ʒu�t�@�C���̍�����\��\n"
		"F:GitViewer=Git�r���A�[\n"
		"F:HideSizeTime=�T�C�Y�ƍX�V�������B��\n"
		"F:ImageViewer=�C���[�W�r���A�[�ŊJ��\n"
		"F:InputDir=���͂����f�B���N�g���ɕύX\n"
		"F:InputPathMask=�p�X�}�X�N�����\n"
		"F:InsSeparator=���[�N���X�g�ɃZ�p���[�^��}��\n"
		"F:ItemTmpDown=���ڂ��ꎞ�I��1���Ɉړ�\n"
		"F:ItemTmpMove=�I�����ڂ��ꎞ�I�ɃJ�[�\���ʒu�Ɉړ�\n"
		"F:ItemTmpUp=���ڂ��ꎞ�I��1��Ɉړ�\n"
		"F:JoinText=�e�L�X�g�t�@�C���̌���\n"
		"F:JumpTo=�w�肵���t�@�C���ʒu��\n"
		"F:Library=���C�u������\n"
		"F:LinkToOpp=�����N��𔽑Α��ɊJ��\n"
		"F:ListArchive=�A�[�J�C�u�̓��e�ꗗ\n"
		"F:ListClipboard=�N���b�v�{�[�h���ꗗ�ŕ\��\n"
		"F:ListDuration=�t�@�C���Đ����Ԃ̈ꗗ\n"
		"F:ListExpFunc=�G�N�X�|�[�g�֐��ꗗ\n"
		"F:ListFileName=�t�@�C�������ꗗ�ŕ\��\n"
		"F:ListNyanFi=NyanFi �ɂ��Ă̏������O�ɕ\��\n"
		"F:ListText=�e�L�X�g�t�@�C�����ꗗ�ŕ\��\n"
		"F:ListTail=�e�L�X�g�t�@�C���̖������ꗗ�ŕ\��\n"
		"F:ListTree=�f�B���N�g���\���̃c���[�\��\n"
		"F:LoadResultList=���ʃ��X�g���t�@�C������ǂݍ���\n"
		"F:LoadTabGroup=�^�u�O���[�v���t�@�C������ǂݍ���\n"
		"F:LockTextPreview=�e�L�X�g�v���r���[�̃��b�N/����\n"
		"F:LogFileInfo=�t�@�C���������O�ɏo��\n"
		"F:MarkMask=�x�}�[�N���ڂ������c���đ����B��\n"
		"F:MaskFind=�w��}�X�N�Ƀ}�b�`���鍀�ڂ�����\n"
		"F:MatchSelect=�w�蕶������܂ރt�@�C����I��\n"
		"F:MonitorOff=�f�B�X�v���C�̓d����؂�\n"
		"F:Move=�ړ�\n"
		"F:MoveTab=�^�u�̈ʒu���ړ�\n"
		"F:MoveTo=�w��f�B���N�g���ֈړ�\n"
		"F:MuteVolume=���ʃ~���[�g\n"
		"F:NameFromClip=�t�@�C�������N���b�v�{�[�h�̓��e��\n"
		"F:NameToLower=�t�@�C�����̏�������\n"
		"F:NameToUpper=�t�@�C�����̑啶����\n"
		"F:NetConnect=�l�b�g���[�N�h���C�u�̊��蓖��\n"
		"F:NetDisconnect=�l�b�g���[�N�h���C�u�̐ؒf\n"
		"F:NewFile=�V�K�t�@�C���̍쐬\n"
		"F:NewTextFile=�V�K�e�L�X�g�t�@�C���̍쐬\n"
		"F:NextDrive=���̃h���C�u��\n"
		"F:NextTab=���̃^�u��\n"
		"F:OpenADS=��փf�[�^�X�g���[�������z���X�g�Ƃ��ĊJ��\n"
		"F:OpenByExp=�G�N�X�v���[���ŊJ��\n"
		"F:OpenCtrlPanel=�R���g���[���p�l�����J��\n"
		"F:OpenGitURL=�����[�g���|�W�g��URL���J��\n"
		"F:OpenStandard=�W���� Enter �L�[����\n"
		"F:OpenTrash=���ݔ����J��\n"
		"F:Pack=���΃p�X�ɃA�[�J�C�u�쐬\n"
		"F:PackToCurr=�J�����g�p�X�ɃA�[�J�C�u�쐬\n"
		"F:Paste=�\��t��\n"
		"F:PathMaskDlg=�p�X�}�X�N�_�C�A���O\n"
		"F:PlayList=�v���C���X�g������čĐ�\n"
		"F:PopDir=�f�B���N�g�����|�b�v\n"
		"F:PopupTab=�^�u�I�����j���[��\��\n"
		"F:PowerOff=Windows���I��\n"
		"F:PowerShell=PowerShell ���N��\n"
		"F:PrevDrive=�O�̃h���C�u��\n"
		"F:PrevTab=�O�̃^�u��\n"
		"F:PushDir=�f�B���N�g�����v�b�V��\n"
		"F:Reboot=Windows���ċN��\n"
		"F:RecentList=�ŋߎg�����t�@�C���ꗗ\n"
		"F:RegDirDlg=�o�^�f�B���N�g���_�C�A���O\n"
		"F:RegDirPopup=�o�^�f�B���N�g���E�|�b�v�A�b�v���j���[\n"
		"F:RegSyncDlg=�����R�s�[�̐ݒ�\n"
		"F:ReloadList=�t�@�C�����X�g���ŐV�̏��ɍX�V\n"
		"F:RenameDlg=���O���̕ύX\n"
		"F:RepositoryList=���|�W�g���ꗗ\n"
		"F:Restart=NyanFi�̍ċN��\n"
		"F:ReturnList=�t�@�C�����X�g�\���ɖ߂�\n"
		"F:SaveAsResultList=���ʃ��X�g�ɖ��O��t���ĕۑ�\n"
		"F:SaveAsTabGroup=�^�u�O���[�v�ɖ��O��t���ĕۑ�\n"
		"F:SaveTabGroup=�^�u�O���[�v���㏑���ۑ�\n"
		"F:ScrollDownLog=���O�����ɃX�N���[��\n"
		"F:ScrollDownText=�e�L�X�g�v���r���[�����ɃX�N���[��\n"
		"F:ScrollUpLog=���O����ɃX�N���[��\n"
		"F:ScrollUpText=�e�L�X�g�v���r���[����ɃX�N���[��\n"
		"F:SelAllItem=���ׂĂ̍��ڂ�I��/����\n"
		"F:SelByList=���X�g�ɂ���č��ڂ�I��\n"
		"F:SelectUp=�I��/������A�J�[�\������Ɉړ�\n"
		"F:SelEmptyDir=��̃f�B���N�g��������I��\n"
		"F:SelGitChanged=Git��ƃf�B���N�g���ŕύX�t�@�C����I��\n"
		"F:SelMask=�I�𒆂̍��ڂ������c���đ����B��\n"
		"F:SelOnlyCur=�J�����g�������ɂ���t�@�C����I��\n"
		"F:SelReverseAll=���ׂĂ̍��ڂ̑I����Ԃ𔽓]\n"
		"F:SetAlias=���[�N���X�g�̍��ڂɃG�C���A�X��ݒ�\n"
		"F:SetArcTime=�A�[�J�C�u�̃^�C���X�^���v���ŐV�ɍ��킹��\n"
		"F:SetDirTime=�f�B���N�g���̃^�C���X�^���v���ŐV�ɍ��킹��\n"
		"F:SetExifTime=�^�C���X�^���v��Exif�B�e�����ɐݒ�\n"
		"F:SetFolderIcon=�t�H���_�A�C�R���̐ݒ�\n"
		"F:SetPathMask=�p�X�}�X�N��ݒ�\n"
		"F:SetSttBarFmt=�X�e�[�^�X�o�[������ݒ�\n"
		"F:SetSubSize=�T�u�E�B���h�E�̃T�C�Y��ݒ�\n"
		"F:ShareList=���L�t�H���_�ꗗ\n"
		"F:ShowByteSize=�t�@�C���T�C�Y���o�C�g�P�ʂŕ\��\n"
		"F:ShowHideAtr=�B���t�@�C����\��\n"
		"F:ShowIcon=�A�C�R���̕\��/��\��\n"
		"F:ShowLogWin=���O�E�B���h�E�[�̕\��/��\��\n"
		"F:ShowPreview=�C���[�W�v���r���[�̕\��/��\��\n"
		"F:ShowProperty=�t�@�C�����̕\��/��\��\n"
		"F:ShowSystemAtr=�V�X�e���t�@�C����\��\n"
		"F:ShowTabBar=�^�u�o�[�̕\��/��\��\n"
		"F:SoloTab=���̃^�u�����ׂč폜\n"
		"F:SpecialDirList=����t�H���_�ꗗ\n"
		"F:SubDirList=�T�u�f�B���N�g���ꗗ\n"
		"F:SwapLR=���E�̃t�@�C�����X�g�����ւ���\n"
		"F:SwapName=���O�����ւ���\n"
		"F:SyncLR=���E�f�B���N�g���̓����ύX��L��/����\n"
		"F:TabDlg=�^�u�̐ݒ�_�C�A���O\n"
		"F:TabHome=�^�u�̃z�[����\n"
		"F:TestArchive=�A�[�J�C�u�̐�����������\n"
		"F:TextViewer=�e�L�X�g�r���A�[�ŊJ��\n"
		"F:ToExViewer=�ʃE�B���h�E�̃e�L�X�g�r���A�[��\n"
		"F:ToLog=���O�E�B���h�E��\n"
		"F:ToNextOnRight=�E�t�@�C�����X�g�Ŏ���NyanFi��\n"
		"F:ToOpposite=���Α��̃t�@�C�����X�g��\n"
		"F:ToOppSameItem=�J�[�\���ʒu�Ɠ����̔��Α����ڂ�\n"
		"F:ToOppSameHash=�J�[�\���ʒu�Ɠ��n�b�V���l�̔��Α����ڂ�\n"
		"F:ToParent=�e�f�B���N�g����\n"
		"F:ToParentOnLeft=���t�@�C�����X�g�Őe�f�B���N�g����\n"
		"F:ToParentOnRight=�E�t�@�C�����X�g�Őe�f�B���N�g����\n"
		"F:ToPrevOnLeft=���t�@�C�����X�g�őO��NyanFi��\n"
		"F:ToRoot=���[�g�f�B���N�g����\n"
		"F:ToTab=�w��ԍ�/�L���v�V�����̃^�u��\n"
		"F:ToText=�e�L�X�g�v���r���[��\n"
		"F:TrimTagData=�^�O�f�[�^�̐���\n"
		"F:UndoRename=���O�̉��������ɖ߂�\n"
		"F:UnPack=���΃p�X�ɉ�\n"
		"F:UnPackToCurr=�J�����g�p�X�ɉ�\n"
		"F:UpdateFromArc=�A�[�J�C�u����X�V\n"
		"F:ViewIniFile=INI�t�@�C���̉{��\n"
		"F:ViewTail=�e�L�X�g�t�@�C���̖������{��\n"
		"F:WatchTail=�e�L�X�g�t�@�C���̒ǉ��X�V���Ď�\n"
		"F:WidenCurList=�J�����g�����X�g�̕����L����\n"
		"F:WinMaximize=�E�B���h�E�̍ő剻\n"
		"F:WinMinimize=�E�B���h�E�̍ŏ���\n"
		"F:WinNormal=�E�B���h�E�����̃T�C�Y�ɖ߂�\n"
		"F:WorkItemDown=���[�N���X�g�̍��ڂ�1���Ɉړ�\n"
		"F:WorkItemMove=�I�����[�N���X�g���ڂ��J�[�\���ʒu�Ɉړ�\n"
		"F:WorkItemUP=���[�N���X�g�̍��ڂ�1��Ɉړ�\n"
		"F:XmlViewer=XML�r���A�[\n"
		"FI:AddTag=���ڂɃ^�O��ǉ�\n"
		"FI:DelTag=���ڂ̃^�O���폜\n"
		"FI:HomeWorkList=�z�[�����[�N���X�g���J��\n"
		"FI:LoadBgImage=�w�i�摜��ǂݍ���\n"
		"FI:LoadWorkList=���[�N���X�g���t�@�C������ǂݍ���\n"
		"FI:MaskSelect=�w��}�X�N�Ƀ}�b�`����t�@�C����I��\n"
		"FI:NewWorkList=�V�K���[�N���X�g�̍쐬\n"
		"FI:NextMark=���̞x�}�[�N���ڂ�\n"
		"FI:PrevMark=�O�̞x�}�[�N���ڂ�\n"
		"FI:NextSameName=�t�@�C�����啔���������t�@�C����\n"
		"FI:OpenByApp=�Ǝ��̊֘A�t���ŊJ��\n"
		"FI:OpenByWin=Windows�̊֘A�t���ŊJ��\n"
		"FI:SaveAsWorkList=���[�N���X�g�ɖ��O��t���ĕۑ�\n"
		"FI:SaveWorkList=���[�N���X�g���㏑���ۑ�\n"
		"FI:SelAllFile=���ׂẴt�@�C����I��/����\n"
		"FI:SelMark=�x�}�[�N���ڂ�I��\n"
		"FI:SelReverse=�t�@�C���̑I����Ԃ𔽓]\n"
		"FI:SelSameExt=�g���q�������t�@�C����I��\n"
		"FI:SelSameName=�t�@�C�����啔�������t�@�C����I��\n"
		"FI:SelWorkItem=���[�N���X�g�̓o�^���ڂ�I��\n"
		"FI:SetTag=���ڂɃ^�O��ݒ�\n"
		"FI:SortDlg=�\�[�g�_�C�A���O\n"
		"FI:SubViewer=�T�u�r���A�[�̕\��/��\��\n"
		"FI:TagSelect=�w��^�O���܂ލ��ڂ�I��\n"
		"FI:UseTrash=�폜�ɂ��ݔ����g��/�g��Ȃ�\n"
		"FI:WorkList=���[�N���X�g\n"
		"FL:CancelAllTask=���ׂẴ^�X�N�𒆒f\n"
		"FL:ClearLog=���O���N���A\n"
		"FL:PauseAllTask=���ׂẴ^�X�N����U��~/�ĊJ\n"
		"FL:Suspend=�^�X�N�\�񍀖ڂ̎��s��ۗ�/����\n"
		"FL:TaskMan=�^�X�N�}�l�[�W��\n"
		"FL:ToLeft=���t�@�C�����X�g��\n"
		"FL:ToRight=�E�t�@�C�����X�g��\n"
		"FL:ViewLog=���O���e�L�X�g�r���A�[�ŕ\��\n"
		"FS:ClearAll=���ׂĂ̑I����Ԃ�����\n"
		"FSI:Select=�I��/����\n"
		"FSVIL:HelpContents=�w���v�̖ڎ�/������\��\n"
		"FSVIL:KeyList=�L�[���蓖�Ĉꗗ\n"
		"FSVIL:OptionDlg=�I�v�V�����ݒ�\n"
		"FV:BinaryEdit=�t�@�C���̃o�C�i���ҏW\n"
		"FV:CursorDownSel=�I�����Ȃ���J�[�\�������Ɉړ�\n"
		"FV:CursorUpSel=�I�����Ȃ���J�[�\������Ɉړ�\n"
		"FV:EditHighlight=�\�������\����`�t�@�C���̕ҏW\n"
		"FV:EditHistory=�ŋߕҏW�����t�@�C���ꗗ\n"
		"FV:Grep=�����񌟍�(GREP)\n"
		"FV:HtmlToText=�e�L�X�g�r���A�[��HTML���e�L�X�g�ϊ��\��\n"
		"FV:IncSearch=�C���N�������^���T�[�`\n"
		"FV:FindTagName=tags ����^�O������\n"
		"FV:FixedLen=�e�L�X�g�r���A�[��CSV/TSV���Œ蒷�\��\n"
		"FV:PageDownSel=�I�����Ȃ���1�y�[�W���Ɉړ�\n"
		"FV:PageUpSel=�I�����Ȃ���1�y�[�W��Ɉړ�\n"
		"FV:RegExChecker=���K�\���`�F�b�J�[\n"
		"FV:SetColor=�e�L�X�g�r���A�[�̔z�F\n"
		"FV:SetFontSize=�t�H���g�T�C�Y��ύX\n"
		"FV:SetMargin=�e�L�X�g�r���A�[�̍����]����ݒ�\n"
		"FV:SetTab=�e�L�X�g�r���A�[�̃^�u�X�g�b�v����ݒ�\n"
		"FV:SetWidth=�e�L�X�g�r���A�[�̐܂�Ԃ�����ݒ�\n"
		"FV:ShowCR=�e�L�X�g�r���A�[�ŉ��s��\��/��\��\n"
		"FV:ShowLineNo=�e�L�X�g�r���A�[�̍s�ԍ���\��/��\��\n"
		"FV:ShowRuby=�e�L�X�g�r���A�[�Ń��r��\��/��\��\n"
		"FV:ShowRuler=�e�L�X�g�r���A�[�̃��[����\��/��\��\n"
		"FV:ShowTAB=�e�L�X�g�r���A�[�Ń^�u��\��/��\��\n"
		"FV:ViewHistory=�ŋ߉{�������t�@�C���ꗗ\n"
		"FV:WebSearch=Web�Ō���\n"
		"FV:ZoomReset=�Y�[��������\n"
		"FVL:CursorDown=�J�[�\�������Ɉړ�\n"
		"FVL:CursorUp=�J�[�\������Ɉړ�\n"
		"FVI:AlphaBlend=���C���E�B���h�E�𓧉ߕ\��\n"
		"FVI:AppList=�A�v���P�[�V�����ꗗ\n"
		"FVI:Calculator=�d��\n"
		"FVI:ClearMark=���ׂĂ̞x�}�[�N������\n"
		"FVI:Close=����\n"
		"FVI:CopyFileInfo=�t�@�C�������N���b�v�{�[�h�ɃR�s�[\n"
		"FVI:CmdFileList=�R�}���h�t�@�C���ꗗ\n"
		"FVI:DebugCmdFile=�R�}���h�t�@�C���̃f�o�b�O���s\n"
		"FVI:Delete=�폜\n"
		"FVI:Duplicate=NyanFi�̓�d�N��\n"
		"FVI:ExeCommands=�w�肵���R�}���h�����s\n"
		"FVI:ExeMenuFile=���j���[�t�@�C���̎��s\n"
		"FVI:ExeToolBtn=�c�[���{�^���̎��s\n"
		"FVI:FileEdit=�t�@�C���̕ҏW\n"
		"FVI:InputCommands=���͂����R�}���h�����s\n"
		"FVI:ListFileInfo=�t�@�C�������_�C�A���O�\��\n"
		"FVI:ListLog=���O���ꗗ�ŕ\��\n"
		"FVI:Mark=�x�}�[�N/����\n"
		"FVI:MarkList=�x�}�[�N�ꗗ\n"
		"FVI:MenuBar=���j���[�o�[�̕\��/��\��\n"
		"FVI:NextNyanFi=����NyanFi���A�N�e�B�u��\n"
		"FVI:PopupMainMenu=���C�����j���[���|�b�v�A�b�v�\��\n"
		"FVI:PrevNyanFi=�O��NyanFi���A�N�e�B�u��\n"
		"FVI:PropertyDlg=�v���p�e�B�_�C�A���O��\��\n"
		"FVI:ScrollDown=���ɃX�N���[��\n"
		"FVI:ScrollUp=��ɃX�N���[��\n"
		"FVI:ShowFileInfo=�t�@�C�����������I�ɕ\��\n"
		"FVI:ShowFKeyBar=�t�@���N�V�����L�[�o�[�̕\��/��\��\n"
		"FVI:ShowStatusBar=�X�e�[�^�X�o�[�̕\��/��\��\n"
		"FVI:ShowToolBar=�c�[���o�[��\��\n"
		"FVI:ToolBarDlg=�c�[���o�[�̐ݒ�\n"
		"FVI:WebMap=�摜��GPS����w��n�_�̒n�}���J��\n"
		"FVI:WinPos=�E�B���h�E�̎l�ӂ�ݒ�\n"
		"FVIL:PageDown=1�y�[�W���Ɉړ�\n"
		"FVIL:PageUp=1�y�[�W��Ɉړ�\n"
		"FVI:ZoomIn=�Y�[���C��\n"
		"FVI:ZoomOut=�Y�[���A�E�g\n"
		"I:ColorPicker=�J���[�s�b�J�[\n"
		"I:DoublePage=���J���\��\n"
		"I:EndFile=�Ō�̃t�@�C���Ɉړ�\n"
		"I:EqualSize=���{�\��\n"
		"I:FittedSize=��ʃt�B�b�g�\��\n"
		"I:FlipHorz=���E���]\n"
		"I:FlipVert=�㉺���]\n"
		"I:FullScreen=�S��ʕ\��\n"
		"I:GifViewer=GIF�r���A�[�̕\��/��\��\n"
		"I:GrayScale=�O���[�X�P�[���\��\n"
		"I:Histogram=�q�X�g�O�����̕\��/��\��\n"
		"I:JumpIndex=�w�肵���C���f�b�N�X�Ɉړ�\n"
		"I:Loupe=���[�y�̕\��/��\��\n"
		"I:NextPage=�T���l�C���̎��y�[�W�Ɉړ�\n"
		"I:PageBind=���J���\���̒Ԃ�������ݒ�\n"
		"I:PrevPage=�T���l�C���̑O�y�[�W�Ɉړ�\n"
		"I:Print=�摜�̈��\n"
		"I:RotateLeft=����90�x��]\n"
		"I:RotateRight=�E��90�x��]\n"
		"I:ScrollLeft=���ɃX�N���[��\n"
		"I:ScrollRight=�E�ɃX�N���[��\n"
		"I:SendToWorkList=���[�N���X�g�ɑ���\n"
		"I:ShowGrid=�摜�����O���b�h�̕\��/��\��\n"
		"I:ShowSeekBar=�V�[�N�o�[�̕\��/��\��\n"
		"I:Sidebar=�T�C�h�o�[�̕\��/��\��\n"
		"I:Thumbnail=�T���l�C���̕\��/��\��\n"
		"I:ThumbnailEx=�T���l�C���̑S�ʕ\��/�ʏ�\��\n"
		"I:TopFile=�擪�t�@�C���Ɉړ�\n"
		"I:WarnHighlight=����ьx��\n"
		"S:ClearIncKeyword=�L�[���[�h���N���A\n"
		"S:IncMatchSelect=�}�b�`���鍀�ڂ����ׂđI��\n"
		"S:IncSearchDown=�}�b�`���鍀�ڂ�����������\n"
		"S:IncSearchExit=�C���N�������^���T�[�`���甲����\n"
		"S:IncSearchUp=�}�b�`���鍀�ڂ����������\n"
		"S:KeywordHistory=�L�[���[�h�������Q��\n"
		"S:MigemoMode=Migomo���[�h�̐؂芷��\n"
		"S:NormalMode=�ʏ�̃T�[�`���[�h�ɖ߂�\n"
		"S:SelectDown=�I��/������A�������Ɍ���\n"
		"V:BackViewHist=�e�L�X�g�r���A�[�̗�����߂�\n"
		"V:BitmapView=�r�b�g�}�b�v�r���[�̕\��/��\��\n"
		"V:BoxSelMode=���`�I�����[�h�̊J�n/����\n"
		"V:ChangeCodePage=�����R�[�h�ύX\n"
		"V:ChangeViewMode=�e�L�X�g/�o�C�i���\���̐؂芷��\n"
		"V:CharInfo=�������̕\��/��\��\n"
		"V:CsvCalc=CSV/TSV���ڂ̏W�v\n"
		"V:CsvGraph=CSV/TSV���ڂ̃O���t\n"
		"V:CsvRecord=CSV/TSV���R�[�h�̕\��/��\��\n"
		"V:CursorLeft=�J�[�\�������Ɉړ�\n"
		"V:CursorLeftSel=�I�����Ȃ���J�[�\�������Ɉړ�\n"
		"V:CursorRight=�J�[�\�����E�Ɉړ�\n"
		"V:CursorRightSel=�I�����Ȃ���J�[�\�����E�Ɉړ�\n"
		"V:ExportCsv=CSV/TSV�G�N�X�|�[�g\n"
		"V:FindDown=�������ɍČ���\n"
		"V:FindLinkDown=�����N����������Ɍ���\n"
		"V:FindLinkUp=�����N���������Ɍ���\n"
		"V:FindMarkDown=�}�[�N�s���������Ɍ���\n"
		"V:FindMarkUp=�}�[�N�s��������Ɍ���\n"
		"V:FindSelDown=�I�𕶎�����������Ɍ���\n"
		"V:FindSelUp=�I�𕶎����������Ɍ���\n"
		"V:FindText=�����񌟍�\n"
		"V:FindUp=������ɍČ���\n"
		"V:FunctionList=�֐��ꗗ\n"
		"V:HelpCurWord=�J�[�\���ʒu�̒P����w��w���v�Ō���\n"
		"V:Highlight=�}�b�`��������\��\n"
		"V:Inspector=�C���X�y�N�^�̕\��/��\��\n"
		"V:ImgPreview=�C���[�W�v���r���[\n"
		"V:JumpLine=�w��s�ԍ��Ɉړ�\n"
		"V:LineEnd=�s���Ɉړ�\n"
		"V:LineEndSel=�I�����Ȃ���s���Ɉړ�\n"
		"V:LineTop=�s���Ɉړ�\n"
		"V:LineTopSel=�I�����Ȃ���s���Ɉړ�\n"
		"V:OpenURL=URL���J��\n"
		"V:ReloadFile=�t�@�C���̍ēǂݍ���\n"
		"V:SaveDump=�_���v���X�g���t�@�C���ɕۑ�\n"
		"V:ScrollCursorDown=�X�N���[�����Ȃ��牺�Ɉړ�\n"
		"V:ScrollCursorUp=�X�N���[�����Ȃ����Ɉړ�\n"
		"V:SearchPair=�Ή����銇�ʂ�����\n"
		"V:SelCurWord=�J�[�\���ʒu�̒P���I��\n"
		"V:SelLine=�J�[�\���s��I��\n"
		"V:SelLineCR=�J�[�\���s�S�̂����s�P�ʂőI��\n"
		"V:SelectAll=���ׂđI��\n"
		"V:SelectFile=�\�����t�@�C����I��/����\n"
		"V:SelectMode=�I�����[�h�̊J�n/����\n"
		"V:SetTopAddr=�擪�A�h���X��ݒ�\n"
		"V:SetUserDefStr=���[�U��`�������ݒ�\n"
		"V:Sort=�e�L�X�g�S�̂����s�P�ʂŃ\�[�g\n"
		"V:SwitchSameName=�t�@�C�����啔���������̃t�@�C���ɐ؂芷��\n"
		"V:SwitchSrcHdr=�w�b�_/�\�[�X�t�@�C���̐؂芷��\n"
		"V:TagJump=�G�f�B�^�Ń^�O�W�����v\n"
		"V:TagJumpDirect=�G�f�B�^�Ń_�C���N�g�^�O�W�����v\n"
		"V:TagView=�r���A�[�Ń^�O�W�����v\n"
		"V:TagViewDirect=�r���A�[�Ń_�C���N�g�^�O�W�����v\n"
		"V:TextEnd=�Ō���Ɉړ�\n"
		"V:TextEndSel=�I�����Ȃ���Ō���Ɉړ�\n"
		"V:TextTop=�擪�Ɉړ�\n"
		"V:TextTopSel=�I�����Ȃ���擪�Ɉړ�\n"
		"V:UserDefList=���[�U��`������ꗗ\n"
		"V:WordLeft=�O�̒P��Ɉړ�\n"
		"V:WordRight=���̒P��Ɉړ�\n"
		"VIL:ClipCopy=�N���b�v�{�[�h�ɃR�s�[\n"
		"VI:NextFile=���̃t�@�C����\��\n"
		"VI:PrevFile=�O�̃t�@�C����\��\n"
		"VL:NextErr=���̃G���[�ʒu��\n"
		"VL:PrevErr=�O�̃G���[�ʒu��\n"
		"InitialSearch=�������T�[�`\n";

	s_list->Clear();
	UnicodeString cmd, tmp;
	for (int i=0; i<c_list->Count; i++) {
		UnicodeString lbuf = c_list->Strings[i];
		if (!contains_s(lbuf, _T(':'))) continue;
		UnicodeString ct = split_tkn(lbuf, ':');
		c_list->Strings[i] = lbuf;
		cmd = get_tkn(lbuf, '=');
		for (int j=1; j<=ct.Length(); j++) {
			s_list->Add(tmp.sprintf(_T("%s:%s"), ct.SubString(j, 1).c_str(), cmd.c_str()));
		}
	}
	s_list->Sort();
}

//---------------------------------------------------------------------------
//�R�}���h�̃p�����[�^�ꗗ���擾
//---------------------------------------------------------------------------
void get_PrmList(
	UnicodeString cmd,		//�R�}���h
	int id_idx,				//"FSVIL" ���[�h�̃C���f�b�N�X(0�x�[�X)
	TStringList *p_list,	//[o] �p�����[�^�ꗗ
	TComboBox *cp)			//�R���{�{�b�N�X	(default = NULL)
{
	if (cp) {
		cp->Clear();
		cp->Style	= csDropDownList;
		cp->Text	= EmptyStr;
		cp->Enabled = false;

		if (cp && contained_wd_i(
			_T("AddTag|CalcDirSize|CalcDirSizeAll|Calculator|ChangeDir|ChangeOppDir|Clone|CloneToCurr|ContextMenu|")
			_T("ConvertHtm2Txt|Copy|CopyTo|CountLines|CreateDir|CursorDown|CursorUp|DateSelect|DebugCmdFile|")
			_T("DistributionDlg|ExeCommands|ExeMenuFile|FileEdit|FileRun|Filter|FindDown|FindFileDirDlg|FindFileDlg|")
			_T("FindTag|FindUp|FTPChmod|Grep|HelpCurWord|ImageViewer|IncSearch|JumpIndex|JumpLine|JumpTo|ListArchive|")
			_T("ListDuration|ListExpFunc|ListTail|ListText|ListTree|LoadBgImage|LoadTabGroup|LoadResultList|LoadWorkList|")
			_T("MaskFind|MaskSelect|MatchSelect|MoveTo|NewTextFile|OpenByApp|OpenByExp|OpenByWin|OpenStandard|OpenURL|")
			_T("Pack|PackToCurr|PlayList|PropertyDlg|RegExChecker|Restart|SaveAsTabGroup|ScrollCursorDown|ScrollCursorUp|")
			_T("ScrollDown|ScrollDownLog|ScrollDownText|ScrollUp|ScrollUpLog|ScrollUpText|SelByList|SetColor|SetDirTime|")
			_T("SetFontSize|SetMargin|SetPathMask|SetSttBarFmt|SetSubSize|SetTab|SetTag|SetUserDefStr|SetWidth|SortDlg|")
			_T("SubDirList|TagJumpDirect|TagSelect|TagViewDirect|TextViewer|ToTab|ViewTail|ToOppSameHash|WatchTail|")
			_T("WidenCurList|WinPos|GitDiff|GitViewer"),
			cmd))
		{
			cp->Enabled = true;
			cp->Style = USAME_TI(cmd, "ImageViewer")? csDropDownList : csDropDown;
		}
	}

	UnicodeString params;

	if (USAME_TI(cmd, "AddTab")) {
		params.USET_T("\nNX : ���^�u�̎��ɑ}��\nPR : ���^�u�̑O�ɑ}��");
	}
	else if (USAME_TI(cmd, "AlphaBlend")) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\r\nIN : �����x�����\r\n");
	}
	else if (USAME_TI(cmd, "AppList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("FA : �ꗗ���Ƀt�H�[�J�X\n")
			_T("FL : �����`���[���Ƀt�H�[�J�X\n")
			_T("FI : �����`���[���Ƀt�H�[�J�X(INC.�T�[�`)\n")
			_T("AO : �ꗗ�̂ݕ\��\n")
			_T("LO : �����`���[�̂ݕ\��\n")
			_T("LI : �����`���[�̂ݕ\��(INC.�T�[�`)\n"));
	}
	else if (USAME_TI(cmd, "BgImgMode")) {
		params.sprintf(_T("%s"),
			_T("OFF : ��\��\n")
			_T("1 : 2��ʂɂ킽���ĕ\��\n")
			_T("2 : ���ꂼ��ɕ\��\n")
			_T("3 : �f�X�N�g�b�v�w�i��\��\n")
			_T("^1 : 2��ʂɂ킽���ĕ\��/��\��\n")
			_T("^2 : ���ꂼ��ɕ\��/��\��\n")
			_T("^3 : �f�X�N�g�b�v�w�i��\��/��\��\n"));
	}
	else if (contained_wd_i(_T("CursorUp|CursorDown|ScrollCursorDown|ScrollCursorUp|ScrollDown|ScrollUp"), cmd)) {
		if (id_idx!=4) params.USET_T("\nHP : ���y�[�W��\nFP : 1�y�[�W��\n");
	}
	else if (contained_wd_i(_T("CalcDirSize|CalcDirSizeAll"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("FC : �t�@�C�����A�T�u�f�B���N�g������\��\n")
			_T("LO : ���ʂ����O�ɏo��\n")
			_T("CC : ���ʂ��N���b�v�{�[�h�ɃR�s�[\n")
			_T("LS : ���ʂ��ꗗ�\��\n")
			_T("SA : ���ʂ��T�C�Y�̏��������Ƀ\�[�g\n")
			_T("SD : ���ʂ��T�C�Y�̑傫�����Ƀ\�[�g\n"));
		if (USAME_TI(cmd, "CalcDirSizeAll")) {
			params.UCAT_T("SG : �O���t�\��(�΃J�����g)\nDG : �O���t�\��(�΃h���C�u)\n");
		}
	}
	else if (USAME_TI(cmd, "Calculator")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("CB : �N���b�v�{�[�h����Čv�Z\n"));
	}
	else if (USAME_TI(cmd, "ChangeCodePage")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("932 :   Shift_JIS\n")
			_T("50220 : JIS(ISO-2022-JP)\n")
			_T("20932 : EUC-JP\n")
			_T("1252 :  Latin-1\n")
			_T("65001 : UTF-8\n")
			_T("1200 :  UTF-16\n"));
	}
	else if (contained_wd_i(_T("ChangeDrive|DriveGraph|EjectDrive"), cmd)) {
		for (int i=0; i<26; i++) params.cat_sprintf(_T("%c\n"), 'A'+i);	//�p��
		if (USAME_TI(cmd, "EjectDrive")) params.UCAT_T(". : �J�����g�h���C�u\n");
		if (USAME_TI(cmd, "DriveGraph")) params = "\n" + params;
	}
	else if (USAME_TI(cmd, "CheckUpdate")) {
		params.USET_T("\nNC : �ۑ��ꏊ�̑I���A�m�F�Ȃ�\n");
	}
	else if (USAME_TI(cmd, "ClearAll")) {
		params.USET_T("\nAL : ���E���ׂĂ̑I����Ԃ�����\n");
	}
	else if (USAME_TI(cmd, "ClearMark")) {
		params.USET_T("\nAC : ���ׂĂ̏ꏊ�̂��ׂẴ}�[�N������\n");
	}
	else if (USAME_TI(cmd, "ClipCopy")) {
		if (id_idx==2) {
			params.USET_T("\nAD : ���݂̓��e�ɒǉ�\n");
		}
		else if (id_idx==3) {
			params.USET_T("\nVI : �\������Ă����ԂŃR�s�[\n");
		}
	}
	else if (USAME_TI(cmd, "Close") && id_idx==2) {
		params.USET_T("\nAL : ���ׂĂ̕ʃE�B���h�E�����\n");
	}
	else if (USAME_TI(cmd, "CompareDlg")) {
		params.USET_T("\nNC : �_�C�A���O���o�����A���O�̂ݔ�r\n");
	}
	else if (contained_wd_i(_T("CompareHash|GetHash|ToOppSameHash"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("MD5\n")
			_T("SHA1\n")
			_T("SHA256\n")
			_T("SHA384\n")
			_T("SHA512\n")
			_T("CRC32\n"));
		if (USAME_TI(cmd, "ToOppSameHash")) params.UCAT_T("NO : ���Α��ֈړ����Ȃ�\n");
		if (USAME_TI(cmd, "CompareHash"))   params.UCAT_T("OS : �s��v�t�@�C���𔽑Α��őI��\n");
	}
	else if (USAME_TI(cmd, "CompressDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("UN : ���k����������\n")
			_T("AL : ���ׂĂ̎�ނ̃t�@�C�������k\n"));
	}
	else if (USAME_TI(cmd, "ConvertDoc2Txt")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("SJ : Shift_JIS �ŏo��(�f�t�H���g)\n")
			_T("IJ : ISO-2022-JP �ŏo��\n")
			_T("EJ : EUC-JP �ŏo��\n")
			_T("U8 : UTF-8 �ŏo��\n")
			_T("UL : UTF-16 �ŏo��\n")
			_T("UB : UTF-16BE �ŏo��\n"));
	}
	else if (USAME_TI(cmd, "ConvertHtm2Txt")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("HD : �w�b�_����}��\n")
			_T("MD : Markdown�L�@�ɕϊ�\n")
			_T("TX : �ʏ�e�L�X�g�ɕϊ�\n"));
	}
	else if (USAME_TI(cmd, "CopyDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("TO : �R�s�[������\n")
			_T("KT : �^�C���X�^���v���ێ�\n")
			_T("CC : �f�B���N�g�������N���b�v�{�[�h�ɃR�s�[\n")
			_T("LS : �f�B���N�g�������ꗗ�\��\n"));
	}
	else if (contained_wd_i(_T("CopyFileName|ListFileName"), cmd)) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\nFN : �t�@�C���������̂�\n");
	}
	else if (USAME_TI(cmd, "CreateDir")) {
		params.USET_T("\r\nIN : �f�t�H���g�����w�肵�ē���\r\n");
	}
	else if (contained_wd_i(_T("CsrDirToOpp|LinkToOpp|SwapLR"), cmd)) {
		params.USET_T("\nTO : ���Α��ֈړ�\n");
	}
	else if (contained_wd_i(_T("CurrFromOpp|CurrToOpp"), cmd)) {
		params.USET_T("\nSL : �I����Ԃ𔽉f\n");
		if (USAME_TI(cmd, "CurrToOpp")) params.UCAT_T("TO : ���Α��ֈړ�\n");
	}
	else if (USAME_TI(cmd, "DeleteADS")) {
		params.USET_T("\nZI : Zone.Identifier �݈̂ێ�\n");
	}
	else if (USAME_TI(cmd, "DelJpgExif")) {
		params.USET_T("\nKT : �^�C���X�^���v���ێ�\n");
	}
	else if (USAME_TI(cmd, "DiffDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("AL : �}�X�N *.*�A�T�u�f�B���N�g�����ΏۂƂ��Ē����ɔ�r���s\n"));
	}
	else if (USAME_TI(cmd, "DirHistory")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("GA : �S�̗�����\��\n")
			_T("GS : �S�̗�����\��(�\�[�g/�d���폜)\n")
			_T("FM : �S�̗������t�B���^����\n")
			_T("AC : �J�����g���̗��������ׂč폜\n")
			_T("GC : �S�̗��������ׂč폜\n"));
	}
	else if (USAME_TI(cmd, "DistributionDlg")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("XC : �m�F�Ȃ��Œ����ɃR�s�[\n")
			_T("XM : �m�F�Ȃ��Œ����Ɉړ�\n")
			_T("SN : �t�@�C�����X�g����}�X�N�ƐU�����ݒ�\n"));
	}
	else if (USAME_TI(cmd, "DotNyanDlg")) {
		params.USET_T("\nRS : �J�����g���� .nyanfi ���ēK�p\n");
	}
	else if (USAME_TI(cmd, "Duplicate")) {
		params.USET_T("\nRA : �Ǘ��҂Ƃ��ē�d�N��\n");
	}
	else if (contained_wd_i(_T("EditHistory|ViewHistory"), cmd)) {
		params.USET_T("\nFF : �t�B���^���Ƀt�H�[�J�X\nAC : ���������ׂď���\n");
	}
	else if (USAME_TI(cmd, "ExeCommandLine")) {
		params.USET_T("\nFN : �J�[�\���ʒu�̃t�@�C���������\nLC : �O��̃R�}���h�������\��\n");
	}
	else if (USAME_TI(cmd, "ExtractIcon")) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\nSI : �X���[���A�C�R���𒊏o\n");
	}
	else if (USAME_TI(cmd, "FileEdit") && id_idx==0) {
		params.USET_T("\nOS : ���Α��őI�𒆂̃t�@�C�����J��\n");
	}
	else if (USAME_TI(cmd, "FileExtList")) {
		params.USET_T("\nCP : �J�[�\���ʒu�̃f�B���N�g�����Ώ�\n");
	}
	else if (USAME_TI(cmd, "Filter")) {
		params.USET_T("\nCS : �召���������\nCA : ���s�O�ɑI���}�X�N������\n");
	}
	else if (contained_wd_i(_T("FindFileDlg|FindFileDirDlg"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("NM : �}�X�N�����\��\n")
			_T("FK : �����ꗓ�Ƀt�H�[�J�X\n")
			_T("R0 : �u�T�u�f�B���N�g���������v�I�t\n")
			_T("R1 : �u�T�u�f�B���N�g���������v�I��\n"));
		if (USAME_TI(cmd, "FindFileDlg")) {
			params.cat_sprintf(_T("%s"),
				_T("X0 : �u�g�������v�I�t\n")
				_T("X1 : �u�g�������v�I��\n")
				_T("A0 : �u�A�[�J�C�u���������v�I�t\n")
				_T("A1 : �u�A�[�J�C�u���������v�I��\n"));
		}
	}
	else if (USAME_TI(cmd, "FindHardLink")) {
		params.USET_T("\nOP : ���ʃ��X�g���甽�Α��֔��f\n");
	}
	else if (USAME_TI(cmd, "FindMark")) {
		params.USET_T("\nAL : ���ׂẴ}�[�N���ڂ�����\n");
	}
	else if (contained_wd_i(_T("FindTag|AddTag|SetTag|TagSelect"), cmd)) {
		params.USET_T("\n; : ���̓{�b�N�X�Ń^�O���w��\n");
		params += usr_TAG->TagNameList->Text;
	}
	else if (USAME_TI(cmd, "FindTagName")) {
		params.USET_T("\nEJ : �e�L�X�g�G�f�B�^�Ń_�C���N�g�^�O�W�����v\n");
		if (id_idx==2) params.UCAT_T("CO : ���݂̃t�@�C���݂̂�����\n");
	}
	else if (contained_wd_i(_T("FindSelDown|FindSelUp"), cmd)) {
		params.USET_T("\nEM : �}�b�`��������\��\n");
	}
	else if (USAME_TI(cmd, "FixTabPath")) {
		params.USET_T("\nON : �J�����g�����Œ�\nOFF : �Œ����\n");
	}
	else if (USAME_TI(cmd, "GitDiff")) {
		params.USET_T("\nHD : HEAD ���獷����\��\nXT : �O��diff�c�[���ŕ\��\n");
	}
	else if (USAME_TI(cmd, "GitViewer")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("N0 : �R�~�b�g���𐔂̐������O��\n")
			_T("N30 : �R�~�b�g���𐔂𐧌�����\n")
			_T("CP : �J�[�\���ʒu�t�@�C���̃R�~�b�g������\��\n"));
	}
	else if (USAME_TI(cmd, "HelpContents")) {
		params.USET_T("\nCI : �R�}���h�̍���\nFI : �R�}���h�̋@�\�ʍ���\nCH : �ύX����\n");
	}
	else if (USAME_TI(cmd, "IncSearch")) {
		params.USET_T("\nMM : Migomo���[�h\nNM : �ʏ탂�[�h\n");
		if (id_idx==0) params.UCAT_T("FM : �t�B���^�}�X�N�E���[�h\nCA : ���s�O�ɑI���}�X�N������\n");
	}
	else if (USAME_TI(cmd, "InputDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ND : �_�C�A���O��\�����Ȃ��œ���\n")
			_T("ND2 : ND �Ńh���b�v�_�E�����J���ĕ\��\n")
			_T("SD : �t�H���_�Q�ƃ_�C�A���O��\��\n"));
	}
	else if (USAME_TI(cmd, "DriveList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ND : �|�b�v�A�b�v���j���[�őI��\n")
			_T("NS : �|�b�v�A�b�v���j���[�őI��(�󂫗e�ʔ�\��)\n"));
	}
	else if (USAME_TI(cmd, "InputCommands")) {
		params.USET_T("\nEL : �G�R�[�A�R�}���h�t�@�C���̍s�ԍ��\��\n");
	}
	else if (contained_wd_i(_T("CmdFileList|ListClipboard|MarkList|KeyList|FunctionList|RepositoryList|UserDefList"), cmd)) {
		params.USET_T("\nFF : �t�B���^���Ƀt�H�[�J�X\n");
	}
	else if (contained_wd_i(_T("Exit|Close"), cmd)) {
		params.USET_T("\nNS : INI�t�@�C����ۑ����Ȃ�\nNX : ����NyanFi���I�������Ȃ�\n");
	}
	else if (USAME_TI(cmd, "ExPopupMenu")) {
		params.USET_T("\nMN : �ǉ����j���[�̂ݕ\��\nTL : �O���c�[���̂ݕ\��\n");
	}
	else if (USAME_TI(cmd, "Library")) {
		params.USET_T("\nSD : �I���_�C�A���O��\��\n* : �I�����j���[��\��\n");
		UnicodeString pnam = cv_env_str("%APPDATA%\\Microsoft\\Windows\\Libraries\\");
		std::unique_ptr<TStringList> lst(new TStringList());
		get_files(pnam, _T("*.library-ms"), lst.get());
		for (int i=0; i<lst->Count; i++)
			params.cat_sprintf(_T("%s\n"), get_base_name(lst->Strings[i]).c_str());
	}
	else if (contained_wd_i(_T("CountLines|ListArchive|ListDuration|ListExpFunc|ListNyanFi"), cmd)) {
		params.USET_T("\nCC : ���ʂ��N���b�v�{�[�h�ɃR�s�[\nLS : ���ʂ��ꗗ�\��\n");
	}
	else if (USAME_TI(cmd, "ListLog")) {
		params.USET_T("\nEO : �G���[�ӏ��̍i�荞�ݕ\��\n");
	}
	else if (USAME_TI(cmd, "ListTail")) {
		params.USET_T("\nTE : �Ō���Ɉړ�r\nFF : �t�B���^���Ƀt�H�[�J�X\n");
	}
	else if (USAME_TI(cmd, "ListText")) {
		params.USET_T("\nFF : �t�B���^���Ƀt�H�[�J�X\nEO : �G���[�ӏ��̍i�荞�ݕ\��\n");
	}
	else if (USAME_TI(cmd, "Mark") && (id_idx==0 || id_idx==3)) {
		params.USET_T("\nND : �J�[�\�����ړ����Ȃ�\nIM : ���������\n");
	}
	else if (USAME_TI(cmd, "MoveTab")) {
		params.USET_T("\nTP : �擪�ɐ擪\nED : �Ō�Ɉړ�\nPR : �P�O�Ɉړ�\n");
	}
	else if (USAME_TI(cmd, "PageBind")) {
		params.USET_T("\nR : �E�Ԃ�\nL : ���Ԃ�\n");
	}
	else if (USAME_TI(cmd, "Paste")) {
		params.USET_T("\nCL : �������ɃN���[����\n");
	}
	else if (contained_wd_i(_T("NextFile|PrevFile"), cmd)) {
		if (id_idx==3) params.USET_T("\nF1 : ���J���\���ł�1�t�@�C�����ړ�\n");
	}
	else if (USAME_TI(cmd, "NextNyanFi")) {
		params.USET_T("\nDN : �Ȃ���Γ�d�N��\n");
	}
	else if (USAME_TI(cmd, "PlayList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("RP : ���s�[�g�Đ�\n")
			_T("SF : �V���b�t���Đ�\n")
			_T("SR : �V���b�t���E���s�[�g�Đ�\n")
			_T("NX : ���̋Ȃ�\n")
			_T("PR : �O�̋Ȃ�\n")
			_T("PS : �ꎞ��~\n")
			_T("RS : �ĊJ\n")
			_T("FI : �t�@�C������\��\n")
			_T("LS : �v���C���X�g��\��\n"));
	}
	else if (USAME_TI(cmd, "RecentList")) {
		params.USET_T("\nAC : �ŋߎg�������ׂĂ̍��ڂ��폜\nBC : �����N�؂ꍀ�ڂ𐮗�\n");
	}
	else if (contained_wd_i(_T("RegDirDlg|PathMaskDlg"), cmd)) {
		params.USET_T("\nND : �|�b�v�A�b�v���j���[�őI��\n");
		if (USAME_TI(cmd, "RegDirDlg")) params.UCAT_T("AD : �ǉ����[�h\n");
	}
	else if (contained_wd_i(_T("RegDirPopup|PushDir|PopDir"), cmd)) {
		params.USET_T("\nOP : ���Α��Ŏ��s\n");
	}
	else if (USAME_TI(cmd, "ReloadList")) {
		params.USET_T("\nCO : �J�����g�̂ݍX�V\nHL : �n�[�h�����N�̃^�C���X�^���v�X�V\nOFF : �X�V�֎~\n");
	}
	else if (USAME_TI(cmd, "Restart")) {
		params.USET_T("\nNS : INI�t�@�C����ۑ����Ȃ�\nRA : �Ǘ��҂Ƃ��čċN��\nDM : �Ǘ��҂���~�i���čċN��\n");
	}
	else if (USAME_TI(cmd, "SaveAsWorkList")) {
		params.USET_T("\nFL : �J�����g�̓��e�����[�N���X�g�Ƃ��ĕۑ�\n");
	}
	else if (contained_wd_i(_T("ScrollUpLog|ScrollUpText|ScrollDownLog|ScrollDownText"), cmd)) {
		params.USET_T("\nHP : ���y�[�W��\nFP : 1�y�[�W��\n");
		params.cat_sprintf(_T("%s\n"), contains_i(cmd, _T("Down"))? _T("ED : �Ō��") : _T("TP : �擪��"));
	}
	else if (USAME_TI(cmd, "SelByList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("LR : �J�����g/���Α��̗����őI��\n")
			_T("SM : �I�����ꂽ���ڂ������c��(�I���}�X�N)\n")
			_T("CP : �J�[�\���ʒu�̃��X�g�t�@�C���ŃJ�����g����I��\n")
			_T("OP : �J�[�\���ʒu�̃��X�g�t�@�C���Ŕ��Α���I��\n"));
	}
	else if (USAME_TI(cmd, "Select") && (id_idx==0 || id_idx==3)) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\n");
		if (id_idx==0) params.UCAT_T("IN : �J��Ԃ��񐔂����\n");
		params.UCAT_T("ND : �J�[�\�����ړ����Ȃ�\n");
	}
	else if (USAME_TI(cmd, "SelectFile")) {
		params.USET_T("\nNX : ���̃t�@�C����\��\n");
	}
	else if (USAME_TI(cmd, "SelEmptyDir")) {
		params.USET_T("\nNF : �t�@�C�����܂܂�Ă��Ȃ���ΑI��\n");
	}
	else if (contained_wd_i(_T("MarkMask|SelMask"), cmd)) {
		params.USET_T("\nCA : �}�X�N������\n");
	}
	else if (USAME_TI(cmd, "SetColor")) {
		params.USET_T("\nRS : �z�F�����Z�b�g\n");
	}
	else if (USAME_TI(cmd, "SetFolderIcon")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ND : �|�b�v�A�b�v���j���[�őI��\n")
			_T("RS : �f�t�H���g�A�C�R���ɖ߂�\n"));
	}
	else if (USAME_TI(cmd, "SetPathMask")) {
		params.USET_T("\nEX : �J�[�\���ʒu�̊g���q�Ń}�X�N\n");
	}
	else if (USAME_TI(cmd, "SetTopAddr")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("TP : �擪�A�h���X��擪��\n")
			_T("NX : �擪�A�h���X���㑱����\n")
			_T("PR : �擪�A�h���X���s����\n")
			_T("ED : �I�[�A�h���X���Ō��\n"));
	}
	else if (USAME_TI(cmd, "ShowFileInfo") && id_idx!=2) {
		params.USET_T("\nSD : �_�C�A���O�ŕ\��\n");
	}
	else if (USAME_TI(cmd, "Sort")) {
		params.USET_T("\nAO : ����\nDO : �~��\n");
	}
	else if (USAME_TI(cmd, "SortDlg")) {
		params.sprintf(_T("%s"),
		 	_T("\n")
			_T("F : ���O��\n")
			_T("E : �g���q��\n")
			_T("D : �X�V������\n")
			_T("S : �T�C�Y��\n")
			_T("A : ������\n")
			_T("U : �Ȃ�\n")
			_T("L : �ꏊ��(���ʃ��X�g)\n")
			_T("FE : ���O/�g���q�� �g�O���؂�ւ�\n")
			_T("FD : ���O/�X�V������ �g�O���؂�ւ�\n")
			_T("FS : ���O/�T�C�Y�� �g�O���؂�ւ�\n")
			_T("ED : �g���q/�X�V������ �g�O���؂�ւ�\n")
			_T("ES : �g���q/�T�C�Y�� �g�O���؂�ւ�\n")
			_T("DS : �X�V������/�T�C�Y�� �g�O���؂�ւ�\n")
			_T("IV : ���݂̃\�[�g���@���t����\n")
			_T("IA : ���ׂẴ\�[�g���@���t����\n")
			_T("XN : �f�B���N�g�� - �t�@�C���Ɠ���\n")
			_T("XF : �f�B���N�g�� - ���O\n")
			_T("XD : �f�B���N�g�� - �X�V����\n")
			_T("XS : �f�B���N�g�� - �T�C�Y\n")
			_T("XA : �f�B���N�g�� - ����\n")
			_T("XX : �f�B���N�g������ʂ��Ȃ�\n")
			_T("XI : �A�C�R��(�t�@�C�������O/�g���q�̏ꍇ)\n")
			_T("XNX : �t�@�C���Ɠ���/�f�B���N�g������ʂ��Ȃ� �g�O���؂�ւ�\n")
			_T("XNI : �t�@�C���Ɠ���/�A�C�R�� �g�O���؂�ւ�\n"));
	}
	else if (USAME_TI(cmd, "SubDirList")) {
		params.USET_T("\nND : �|�b�v�A�b�v���j���[�őI��\n");
	}
	else if (USAME_TI(cmd, "SwapName")) {
		params.USET_T("\nLR : ���E�œ���ւ�\n");
	}
	else if (USAME_TI(cmd, "TabHome")) {
		params.USET_T("\nAL : ���ׂẴ^�u�ɓK�p\nCO : �J�����g�݂̂ɓK�p\n");
	}
	else if (contained_wd_i("TagJump|TagView", cmd)) {
		params.USET_T("\nDJ : ������Ȃ���΃_�C���N�g�^�O�W�����v\n");
	}
	else if (contained_wd_i(_T("TextViewer|ImageViewer"), cmd)) {
		params.USET_T("\nCB : �N���b�v�{�[�h�̓��e��\��\nNN : ����NyanFi�ŕ\��\n");
		if (USAME_TI(cmd, "TextViewer")) params.UCAT_T("XW : �ʃE�B���h�E�ŕ\��\n");
	}
	else if (USAME_TI(cmd, "ToOppSameItem")) {
		params.USET_T("\nNO : ���Α��ֈړ����Ȃ�\n");
	}
	else if (contained_wd_i(_T("ToParent|ToParentOnLeft|ToParentOnRight"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("DL : ���[�g�Ȃ�h���C�u/���L�t�H���_�ꗗ��\��\n")
			_T("DP : ���[�g�Ȃ�h���C�u�I�����j���[��\��\n"));
	}
	else if (contained_wd_i(_T("UnPack|UnPackToCurr"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("CD : �f�B���N�g�����쐬���Ă��̒���\n")
			_T("CD2 : ���[�g�ɕ����̑Ώۂ���������f�B���N�g���쐬\n"));
	}
	else if (USAME_TI(cmd, "UpdateFromArc")) {
		params.USET_T("\nUN : �V�����A�[�J�C�u��T���čX�V\n");
	}
	else if (contained_wd_i("ViewIniFile|ViewLog", cmd)) {
		params.UCAT_T("\nXW : �ʃE�B���h�E�ŕ\��\n");
	}
	else if (USAME_TI(cmd, "WatchTail")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ST : �Ď����e��\��\n")
			_T("CC : �J�[�\���ʒu�t�@�C���̊Ď��𒆎~\n")
			_T("AC : ���ׂĂ̊Ď��𒆎~\n"));
	}
	if (USAME_TI(cmd, "WebMap")) {
		params.USET_T("\nIN : �ܓx,�o�x�����\n");
	}
	else if (USAME_TI(cmd, "WebSearch")) {
		params.USET_T("\nCB : �N���b�v�{�[�h�̓��e������\n");
		if (id_idx==0) params.UCAT_T("FN : �J�[�\���ʒu�̃t�@�C����������\n");
	}
	else if (USAME_TI(cmd, "WorkList") && id_idx==0) {
		params.USET_T("\nOP : ���Α��Ŏ��s\nRL : �ύX���e��j�����ēǂݍ��ݒ���\nDI : �����ȍ��ڂ��ꊇ�폜\n");
	}
	else if (contained_wd_i(_T("ZoomIn|ZoomOut"), cmd)) {
		params.USET_T("\n");
		for (int i=2; i<=12; i++) params.cat_sprintf(_T("%u\n"), i);
	}
	else if (contained_wd_i(_T("Copy|Move|Delete|CompleteDelete"), cmd)) {
		params.USET_T("\nSO : �I�����ڂ̂ݏ���\n");
		if (USAME_TI(cmd, "Copy")) {
			params.cat_sprintf(_T("%s"),
				_T("OP : ���Α��R�s�[��̃J�[�\���ʒu��ݒ�\nOP2 : �R�s�[��̃J�[�\���ʒu�𒀎��ݒ�\n")
			 	_T("TO : �R�s�[������\nSD : �R�s�[����Q��\nSS : �J�����g�̃T�u�f�B���N�g����I��\n"));
		}
		if (USAME_TI(cmd, "Move")) {
			params.cat_sprintf(_T("%s"),
				_T("OP : ���Α��ړ���̃J�[�\���ʒu��ݒ�\nOP2 : �ړ���̃J�[�\���ʒu�𒀎��ݒ�\n")
			 	_T("TO : �ړ�������\nSD : �ړ�����Q��\nSS : �J�����g�̃T�u�f�B���N�g����I��\n"));
		}
	}
	//�g�O������R�}���h
	else if (contained_wd_i(
		_T("FileListOnly|HideSizeTime|LockTextPreview|MenuBar|MuteVolume|")
		_T("ShowByteSize|ShowFKeyBar|ShowHideAtr|ShowIcon|ShowPreview|ShowProperty|ShowStatusBar|ShowSystemAtr|ShowTabBar|")
		_T("PauseAllTask|Suspend|SyncLR|UseTrash|")
		_T("BitmapView|CharInfo|CsvRecord|Highlight|HtmlToText|Inspector|ShowCR|ShowLineNo|ShowRuby|ShowRuler|ShowTAB|FixedLen|")
		_T("DoublePage|FullScreen|GifViewer|SubViewer|GrayScale|Histogram|Loupe|ShowGrid|ShowSeekBar|Sidebar|")
		_T("Thumbnail|ThumbnailEx|WarnHighlight"),
		cmd))
	{
		params.USET_T("\nON : �\��/�L��\nOFF : ��\��/����/����\n");
		if (USAME_TI(cmd, "SubViewer"))
			params.UCAT_T("LK : ���b�N/����\nRL : ����90�x��]\nRR : �E��90�x��]\nFH : ���E���]\nFV : �㉺���]\n");
		if (USAME_TI(cmd, "ShowIcon"))
			params.UCAT_T("FD : �S�\��/�t�H���_�A�C�R���̂ݕ\��\n");
	}
	//�I�v�V�����ݒ�
	else if (USAME_TI(cmd, "OptionDlg")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("GN : ���\n")
			_T("MO : �}�E�X����\n")
			_T("DS : �f�U�C��\n")
			_T("TV : �e�L�X�g�r���A�[\n")
			_T("IV : �C���[�W�r���A�[\n")
			_T("ED : �G�f�B�^\n")
			_T("AC : �֘A�t��\n")
			_T("ST : �N����\n")
			_T("XM : �ǉ����j���[\n")
			_T("XT : �O���c�[��\n")
			_T("KY : �L�[�ݒ�\n")
			_T("NT : �ʒm�E�m�F�E�q���g\n")
			_T("CM : �R�}���h\n")
			_T("EV : �C�x���g\n"));
	}

	p_list->Text = params;
	if (!cp && p_list->Count>0) {
		if (p_list->Strings[0].IsEmpty()) p_list->Delete(0);
	}
}

//---------------------------------------------------------------------------
//�t�@�C��/�f�B���N�g���Q�Ƃ��K�v��?
//---------------------------------------------------------------------------
bool need_RefDirFile(UnicodeString cmd)
{
	return contained_wd_i(
		_T("CalcDirSize|CalcDirSizeAll|ChangeDir|ChangeOppDir|ContextMenu|CopyTo|DebugCmdFile|DistributionDlg|")
		_T("ExeCommands|ExeMenuFile|FileEdit|FileRun|JumpTo|ListArchive|ListDuration|ListExpFunc|ListTail|")
		_T("ListText|ListTree|LoadBgImage|LoadTabGroup|LoadResultList|LoadWorkList|MoveTo|OpenByApp|OpenByExp|")
		_T("OpenByWin|OpenStandard|PropertyDlg|PlayList|TextViewer|SelByList|SetColor|SetFolderIcon|SubDirList|")
		_T("HelpCurWord|Restart"),
		cmd);
}

//---------------------------------------------------------------------------
//�R�}���h�p�����[�^���������������폜
//---------------------------------------------------------------------------
UnicodeString del_CmdDesc(UnicodeString cmd)
{
	UnicodeString prm = get_PrmStr(cmd);
	cmd = get_CmdStr(cmd);
	split_dsc(prm);
	if (!prm.IsEmpty()) cmd.cat_sprintf(_T("_%s"), prm.c_str());
	return cmd;
}

//---------------------------------------------------------------------------
//�J�[�\���L�[���R�}���h�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString get_CsrKeyCmd(UnicodeString key_str)
{
	switch (idx_of_word_i(_T("DOWN|UP|LEFT|RIGHT"), key_str)) {
	case  0: return "CursorDown";
	case  1: return "CursorUp";
	case  2: return "ToLeft";
	case  3: return "ToRight";
	default: return EmptyStr;
	}
}
//---------------------------------------------------------------------------
