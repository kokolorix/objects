/*    ==Scripting Parameters==

    Source Server Version : SQL Server 2016 (13.0.4001)
    Source Database Engine Edition : Microsoft SQL Server Express Edition
    Source Database Engine Type : Standalone SQL Server

    Target Server Version : SQL Server 2017
    Target Database Engine Edition : Microsoft SQL Server Standard Edition
    Target Database Engine Type : Standalone SQL Server
*/
USE [master]
GO
/****** Object:  Database [objects]    Script Date: 04.09.2017 22:29:56 ******/
CREATE DATABASE [objects]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'objects', FILENAME = N'D:\Microsoft SQL Server\MSSQL13.SRDEV\MSSQL\DATA\objects.mdf' , SIZE = 8192KB , MAXSIZE = UNLIMITED, FILEGROWTH = 65536KB )
 LOG ON 
( NAME = N'objects_log', FILENAME = N'D:\Microsoft SQL Server\MSSQL13.SRDEV\MSSQL\DATA\objects_log.ldf' , SIZE = 8192KB , MAXSIZE = 2048GB , FILEGROWTH = 65536KB )
GO
ALTER DATABASE [objects] SET COMPATIBILITY_LEVEL = 130
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [objects].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [objects] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [objects] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [objects] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [objects] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [objects] SET ARITHABORT OFF 
GO
ALTER DATABASE [objects] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [objects] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [objects] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [objects] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [objects] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [objects] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [objects] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [objects] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [objects] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [objects] SET  DISABLE_BROKER 
GO
ALTER DATABASE [objects] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [objects] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [objects] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [objects] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [objects] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [objects] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [objects] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [objects] SET RECOVERY SIMPLE 
GO
ALTER DATABASE [objects] SET  MULTI_USER 
GO
ALTER DATABASE [objects] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [objects] SET DB_CHAINING OFF 
GO
ALTER DATABASE [objects] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [objects] SET TARGET_RECOVERY_TIME = 60 SECONDS 
GO
ALTER DATABASE [objects] SET DELAYED_DURABILITY = DISABLED 
GO
ALTER DATABASE [objects] SET QUERY_STORE = OFF
GO
USE [objects]
GO
ALTER DATABASE SCOPED CONFIGURATION SET LEGACY_CARDINALITY_ESTIMATION = OFF;
GO
ALTER DATABASE SCOPED CONFIGURATION FOR SECONDARY SET LEGACY_CARDINALITY_ESTIMATION = PRIMARY;
GO
ALTER DATABASE SCOPED CONFIGURATION SET MAXDOP = 0;
GO
ALTER DATABASE SCOPED CONFIGURATION FOR SECONDARY SET MAXDOP = PRIMARY;
GO
ALTER DATABASE SCOPED CONFIGURATION SET PARAMETER_SNIFFING = ON;
GO
ALTER DATABASE SCOPED CONFIGURATION FOR SECONDARY SET PARAMETER_SNIFFING = PRIMARY;
GO
ALTER DATABASE SCOPED CONFIGURATION SET QUERY_OPTIMIZER_HOTFIXES = OFF;
GO
ALTER DATABASE SCOPED CONFIGURATION FOR SECONDARY SET QUERY_OPTIMIZER_HOTFIXES = PRIMARY;
GO
USE [objects]
GO
/****** Object:  UserDefinedDataType [dbo].[STRING]    Script Date: 04.09.2017 22:29:57 ******/
CREATE TYPE [dbo].[STRING] FROM [varchar](max) NULL
GO
/****** Object:  Table [dbo].[object]    Script Date: 04.09.2017 22:29:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[object](
	[id] [int] NOT NULL,
	[name] [varchar](max) NULL,
	[type] [int] NULL,
	[version] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[property]    Script Date: 04.09.2017 22:29:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[property](
	[id] [int] NOT NULL,
	[name] [varchar](max) NULL,
	[object] [int] NOT NULL,
	[version] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[value]    Script Date: 04.09.2017 22:29:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[value](
	[id] [int] NOT NULL,
	[type] [int] NULL,
	[property] [int] NULL,
	[value] [varchar](max) NULL,
	[parent] [int] NULL,
	[version] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  View [dbo].[objects]    Script Date: 04.09.2017 22:29:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE VIEW [dbo].[objects]
AS
SELECT        dbo.object.id AS obj_id, dbo.object.name AS obj_name, dbo.object.type AS obj_type, dbo.property.id AS prop_id, dbo.property.name AS prop_name, dbo.value.value, dbo.value.type
FROM            dbo.object INNER JOIN
                         dbo.property ON dbo.object.id = dbo.property.object INNER JOIN
                         dbo.value ON dbo.property.id = dbo.value.property
GO
ALTER TABLE [dbo].[object] ADD  DEFAULT ((1)) FOR [version]
GO
ALTER TABLE [dbo].[property] ADD  DEFAULT ((1)) FOR [version]
GO
ALTER TABLE [dbo].[value] ADD  DEFAULT ((1)) FOR [version]
GO
ALTER TABLE [dbo].[object]  WITH CHECK ADD FOREIGN KEY([type])
REFERENCES [dbo].[object] ([id])
GO
ALTER TABLE [dbo].[property]  WITH CHECK ADD FOREIGN KEY([object])
REFERENCES [dbo].[object] ([id])
GO
ALTER TABLE [dbo].[value]  WITH CHECK ADD FOREIGN KEY([parent])
REFERENCES [dbo].[value] ([id])
GO
ALTER TABLE [dbo].[value]  WITH CHECK ADD FOREIGN KEY([property])
REFERENCES [dbo].[property] ([id])
GO
ALTER TABLE [dbo].[value]  WITH CHECK ADD FOREIGN KEY([type])
REFERENCES [dbo].[object] ([id])
GO
EXEC sys.sp_addextendedproperty @name=N'MS_DiagramPane1', @value=N'[0E232FF0-B466-11cf-A24F-00AA00A3EFFF, 1.00]
Begin DesignProperties = 
   Begin PaneConfigurations = 
      Begin PaneConfiguration = 0
         NumPanes = 4
         Configuration = "(H (1[21] 4[42] 2[17] 3) )"
      End
      Begin PaneConfiguration = 1
         NumPanes = 3
         Configuration = "(H (1 [50] 4 [25] 3))"
      End
      Begin PaneConfiguration = 2
         NumPanes = 3
         Configuration = "(H (1 [50] 2 [25] 3))"
      End
      Begin PaneConfiguration = 3
         NumPanes = 3
         Configuration = "(H (4 [30] 2 [40] 3))"
      End
      Begin PaneConfiguration = 4
         NumPanes = 2
         Configuration = "(H (1 [56] 3))"
      End
      Begin PaneConfiguration = 5
         NumPanes = 2
         Configuration = "(H (2 [66] 3))"
      End
      Begin PaneConfiguration = 6
         NumPanes = 2
         Configuration = "(H (4 [50] 3))"
      End
      Begin PaneConfiguration = 7
         NumPanes = 1
         Configuration = "(V (3))"
      End
      Begin PaneConfiguration = 8
         NumPanes = 3
         Configuration = "(H (1[56] 4[18] 2) )"
      End
      Begin PaneConfiguration = 9
         NumPanes = 2
         Configuration = "(H (1 [75] 4))"
      End
      Begin PaneConfiguration = 10
         NumPanes = 2
         Configuration = "(H (1[66] 2) )"
      End
      Begin PaneConfiguration = 11
         NumPanes = 2
         Configuration = "(H (4 [60] 2))"
      End
      Begin PaneConfiguration = 12
         NumPanes = 1
         Configuration = "(H (1) )"
      End
      Begin PaneConfiguration = 13
         NumPanes = 1
         Configuration = "(V (4))"
      End
      Begin PaneConfiguration = 14
         NumPanes = 1
         Configuration = "(V (2))"
      End
      ActivePaneConfig = 0
   End
   Begin DiagramPane = 
      Begin Origin = 
         Top = 0
         Left = 0
      End
      Begin Tables = 
         Begin Table = "object"
            Begin Extent = 
               Top = 6
               Left = 38
               Bottom = 136
               Right = 208
            End
            DisplayFlags = 280
            TopColumn = 0
         End
         Begin Table = "property"
            Begin Extent = 
               Top = 33
               Left = 277
               Bottom = 163
               Right = 447
            End
            DisplayFlags = 280
            TopColumn = 0
         End
         Begin Table = "value"
            Begin Extent = 
               Top = 105
               Left = 512
               Bottom = 235
               Right = 682
            End
            DisplayFlags = 280
            TopColumn = 0
         End
      End
   End
   Begin SQLPane = 
   End
   Begin DataPane = 
      Begin ParameterDefaults = ""
      End
      Begin ColumnWidths = 17
         Width = 284
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
         Width = 1500
      End
   End
   Begin CriteriaPane = 
      Begin ColumnWidths = 11
         Column = 1440
         Alias = 900
         Table = 1170
         Output = 720
         Append = 1400
         NewValue = 1170
         SortType = 1350
         SortOrder = 1410
         GroupBy = 1350
         Filter = 1350
         Or = 1350
         Or = 1350
         Or = 1350
      End
   End
End
' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'VIEW',@level1name=N'objects'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_DiagramPaneCount', @value=1 , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'VIEW',@level1name=N'objects'
GO
USE [master]
GO
ALTER DATABASE [objects] SET  READ_WRITE 
GO
