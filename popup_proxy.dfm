object FormProxy: TFormProxy
  Left = 0
  Top = 0
  Caption = 'Proxy'
  ClientHeight = 247
  ClientWidth = 430
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Icon.Data = {
    0000010001001010000001002000680400001600000028000000100000002000
    0000010020000000000000040000120B0000120B00000000000000000000FFFF
    FFFFFFFFFFFFFEFEFEFFFDFDFCFFFDFDFCFFFDFDFCFFFDFDFBFFFDFDFCFFFFFF
    FFFFFFFFFFFFCED6BDFFB4C09BFFBCC5AAFFF6F7F4FFFFFFFFFFFDFDFCFFFFFF
    FFFFFEFEFEFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFEFFFFFFFFFFEFF3
    ECFFABBD8DFF84A055FF85A055FF7C964DFF8C9E6CFFDEE3D8FFFFFFFFFFFEFE
    FEFFFFFFFFFFF3F4EFFFC7D0B5FFBAC5A6FFB7C095FFBABF85FFB9C088FF96A7
    5BFF869B43FF90A351FF8DA04FFF8DA150FF829643FF798942FFA2AA76FFFFFF
    FFFFE1E7D5FF90A764FF809A4CFF829E52FF829238FF7D7625FF817D42FFACA2
    3BFF736C23FF6F6921FF7D782BFF716E33FF757134FF78742BFF878118FFF1F4
    EAFF9AAF6EFF86A153FF8FA75FFF92AB5FFF7F955EFF25283AFF38393BFFA69F
    60FFA8A491FFA5A293FFB9B261FFC2B622FFC4B936FFCEC55AFFC1B529FFBDCB
    A0FF88A256FF8EA65DFF8CA45AFF8BA65EFF9BA049FF797121FF6D6825FFB0A6
    2EFFAEA760FFACA663FF777239FF554F1CFF5E5929FF5D5933FF5B5622FFA7BA
    81FF88A256FF8DA65CFF8CA55AFF88A461FFA9A635FF8F842FFF4B482FFFB2A6
    11FF776E0DFFA39810FF4A4514FF0F1021FF1C1B1CFF1A191AFF191920FFA6B9
    80FF88A256FF8DA65CFF8CA55AFF8BA661FF9E9D3BFF978D24FF988F17FFAEA4
    33FF8D8752FFACA331FF9A9329FFA39714FF9E9530FF9E952BFFA49817FFA2B6
    7BFF89A357FF8DA55CFF8CA55AFF89A65CFF969634FF76712CFF978C0CFFBDB7
    70FFD2CFB4FFB6AF64FFA89D2EFFA89A02FFA69A0CFFA69A0CFFB0A40FFFA2B6
    7AFF89A357FF8DA65DFF8AA456FF9CAB81FFB2AF8BFFBFB986FFAEA64BFFA399
    18FF625D20FF98932DFFA6A63AFFB2A526FFB4A92CFFB0A41CFFA99D0BFFA5B8
    7EFF89A256FF8EA65EFF88A353FFB2B8A0FFCDC8D6FFC6C6DAFFB4AF81FFA098
    1FFF847B25FF9A9634FF99A048FFD1C872FFDAD69AFFC6BE5AFFB6AB2BFFA6B9
    80FF88A256FF8DA65DFF89A456FFA5B18BFFBBBAA4FFC7C4AEFFB4AE65FF8E86
    1EFFA0973AFFAFA630FFABAB4AFFCBBF4CFFCAC155FFC5BC4CFFC8BF4EFFAFC0
    8DFF88A255FF8EA65DFF8DA55BFF89A65EFF929B37FF9C9264FF9A9247FF7F79
    48FF424440FF908A33FF9F972FFF9C942BFF9F9529FFA49921FFA69D3EFFDCE3
    CCFF8DA65CFF88A255FF8CA55BFF8BA55DFF93A24DFF868B65FF8D924FFFB5B9
    5AFF898E53FF939371FF9B9A7CFF90916EFF737B43FF798033FF6F773EFFFFFF
    FFFFD6DEC3FF9AB06FFF8DA55CFF8CA55AFF8BA55CFF8DA757FF8CA75CFFABBE
    8CFFF0F5E7FFFFFFFFFFFEFEFEFFFFFFFFFFE6EDD5FFA5BA7BFF90AA5AFFFEFE
    FEFFFFFFFFFFF3F6EEFFE1E8D4FFDEE5CFFFDAE2C9FFDDE4CFFFE3E9D7FFFBFC
    F9FFFFFFFFFFFCFCFCFFFCFCFCFFFDFDFDFFFFFFFFFFF4F6EFFFDEE5D2FF0000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000}
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object LabelProxy: TLabel
    Left = 10
    Top = 10
    Width = 45
    Height = 23
    Caption = 'Proxy'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelProxyPort: TLabel
    Left = 10
    Top = 55
    Width = 33
    Height = 23
    Caption = 'Port'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelProxyUser: TLabel
    Left = 10
    Top = 100
    Width = 82
    Height = 23
    Caption = 'Utilisateur'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelProxyMdp: TLabel
    Left = 10
    Top = 145
    Width = 111
    Height = 23
    Caption = 'Mot de passe'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object EditProxyPort: TEdit
    Left = 180
    Top = 55
    Width = 240
    Height = 24
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object EditProxyUser: TEdit
    Left = 180
    Top = 100
    Width = 240
    Height = 24
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object ButtonProxyOK: TButton
    Left = 175
    Top = 200
    Width = 80
    Height = 40
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = ButtonProxyOKClick
  end
  object EditProxyMdP: TEdit
    Left = 180
    Top = 145
    Width = 240
    Height = 24
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
end
