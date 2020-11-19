USE [NHModeServerDB_GB2018]
GO

/****** Object:  Table [dbo].[WhiteModifiedParameters]    Script Date: 11/14/2020 12:01:39 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[WhiteModifiedParameters](
	[AutoID] [int] IDENTITY(1,1) NOT NULL,
	[A] [nvarchar](50) NOT NULL,
	[B] [nvarchar](50) NULL,
	[C] [nvarchar](50) NULL,
	[D] [nvarchar](50) NULL,
	[E] [nvarchar](50) NULL,
	[F] [nvarchar](50) NULL,
	[G] [nvarchar](50) NULL,
	[H] [nvarchar](50) NULL,
	[I] [nvarchar](50) NULL,
	[J] [nvarchar](50) NULL,
	[K] [nvarchar](50) NULL,
	[L] [nvarchar](50) NULL,
	[M] [nvarchar](50) NULL,
	[N] [nvarchar](50) NULL,
	[O] [nvarchar](50) NULL,
	[P] [nvarchar](50) NULL,
	[Q] [nvarchar](50) NULL,
	[R] [nvarchar](50) NULL,
	[S] [nvarchar](50) NULL,
	[T] [nvarchar](50) NULL,
	[U] [nvarchar](50) NULL,
	[V] [nvarchar](50) NULL,
	[W] [nvarchar](50) NULL,
	[X] [nvarchar](50) NULL,
	[Y] [nvarchar](50) NULL,
	[Z] [nvarchar](50) NULL,
 CONSTRAINT [PK_WhiteModifiedParameters] PRIMARY KEY CLUSTERED 
(
	[A] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO


