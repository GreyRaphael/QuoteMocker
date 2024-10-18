#ifndef __quote_data_h__
#define __quote_data_h__

//
//
//  行情数据基本结构
//
//

#include <cstdint>

namespace eqapi
{
#pragma pack(1)

	enum Quote_Data_Type : std::uint16_t
	{
		Quote_Data_Snapshot = 0x0000,
		Quote_Data_Snapshot_L1 = 0x0001,
		Quote_Data_Snapshot_L2 = 0x0002,
		Quote_Data_Snapshot_Index = 0x0003,
		Quote_Data_Snapshot_Future = 0x0004,
		Quote_Data_Snapshot_Sgex = 0x0005,
		Quote_Data_Snapshot_L2_Bond = 0x0006,
		Quote_Data_Snapshot_L1_Bond = 0x0007,
		Quote_Data_Snapshot_L2_Hkex = 0x0011,
		Quote_Data_Snapshot_Nq = 0x0012,
		Quote_Data_Snapshot_Nqzs = 0x0013,
		Quote_Data_Snapshot_Cnsi = 0x0014,
		Quote_Data_Snapshot_Main_Hkex = 0x0015,
		Quote_Data_Trade = 0x0100,
		Quote_Data_Trade_Bond = 0x0101,
		Quote_Data_Order = 0x0200,
		Quote_Data_Order_Bond = 0x0201,
		Quote_Data_Kline = 0x0300,
		Quote_Data_Indic = 0x8000,
		Quote_Data_Indic_MA = 0x8001,
		Quote_Data_Indic_KDJ = 0x8002,
		Quote_Data_String = 0xFFFE,
		Quote_Data_Unknown = 0xFFFF
	};


	struct Quote_Msg_Head
	{
		std::int8_t mode;                       // data mode, 0 = json; 1 = binary
		std::int8_t compress;                   // quote compress. 0 = no compress, 1 = snappy, 2 = gzip
		std::uint16_t quote_data_type;
		std::int32_t quote_data_len;            // 
	};

	struct Snapshot_L2
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		char instrument_status[8];
		char tpc[8];
		std::int64_t offer_price[10];
		std::int64_t offer_volume[10];
		std::int64_t offer_num_order[10];
		std::int64_t bid_price[10];
		std::int64_t bid_volume[10];
		std::int64_t bid_num_order[10];
		std::int64_t num_trades;
		std::int64_t total_volume_trade;
		std::int64_t total_value_trade;
		std::int64_t total_offer_quant;
		std::int64_t total_bid_quant;                   // 买入总量
		std::int64_t weighted_avg_offer_price;
		std::int64_t weighted_avg_bid_price;
		std::int64_t altWeighted_avg_offer_price;
		std::int64_t altWeighted_avg_bid_price;
		std::int64_t num_offer_order;
		std::int64_t num_bid_order;                     // 买方委托价位数
		std::int64_t high_limited;
		std::int64_t low_limited;
		std::int64_t withdraw_sell_num;              //期权集中竞价交易熔断参考价格 （暂用）
		std::int64_t withdraw_sell_amount;
		std::int64_t withdraw_sell_money;              
		std::int64_t withdraw_buy_num;       
		std::int64_t withdraw_buy_amount;
		std::int64_t withdraw_buy_money;
		std::int64_t total_offer_num;
		std::int64_t total_bid_num;                     // 买入总笔数
		std::int64_t offer_trade_max_duration;
		std::int64_t bid_trade_max_duration;
		std::int64_t etf_buy_num;
		std::int64_t etf_buy_amount;
		std::int64_t etf_buy_money;
		std::int64_t etf_sell_num;
		std::int64_t etf_sell_amount;
		std::int64_t etf_sell_money;
		std::int64_t iopv;
		std::int64_t nav;
		std::int64_t position;
		std::int64_t yield_to_maturity;
		std::int64_t pratio1;
		std::int64_t pratio2;
		std::int64_t updown1;
		std::int64_t updown2;
		std::int64_t weighted_avg_price;                // 上交所债券加权平均回购利率  & 深交所债券现券交易成交量加权平均价
		std::int64_t reserved2;                         // 匹配成交最新价
		std::int64_t reserved3;                         // 匹配成交成交量
		std::int64_t reserved4;                         // 匹配成交成交金额
		std::int32_t offer_one_order[50];
		std::int32_t bid_one_order[50];
	};

	struct Snapshot_L2_Bond       //深交所债券快照
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t last_trade_type;                 // 债券最新价成交方式，1=匹配成交，2 = 协商成交，3 = 点击成交，4 = 询价成交，5 = 竞买成交
		std::int64_t close;
		char tpc[8];
		std::int64_t offer_price[10];
		std::int64_t offer_volume[10];
		std::int64_t offer_num_order[10];
		std::int64_t bid_price[10];
		std::int64_t bid_volume[10];
		std::int64_t bid_num_order[10];
		std::int64_t num_trades;
		std::int64_t total_volume_trade;
		std::int64_t total_value_trade;
		std::int64_t total_offer_quant;                 // 卖出总量
		std::int64_t total_bid_quant;                   // 买入总量
		std::int64_t weighted_avg_offer_price;
		std::int64_t weighted_avg_bid_price;

		std::int64_t updown1;
		std::int64_t updown2;
		std::int64_t weighted_avg_price;                // 上交所债券加权平均回购利率 & 深交所债券现券交易成交量加权平均价
		std::int32_t offer_one_order[50];
		std::int32_t bid_one_order[50];

		std::int64_t weighted_avg_rate;               // 深交所质押式回购的实时成交量加权平均利率加权平均利率
		std::int64_t weighted_avg_rate_updown;        // 质押式回购的加权平均利率涨跌BP
		std::int64_t preclose_weighted_average_rate;  // 昨收盘成交量加权平均利率
		std::int64_t auction_trade_last;        // 匹配成交最新成交价
		std::int64_t auction_trade_volume;            // 匹配成交成交量
		std::int64_t auction_trade_value;             // 匹配成交成交金额
		char sub_tpc[5][8];                           // 细分交易阶段代码

		//std::int64_t auction_trade_last;              //匹配成交最新价
		//std::int64_t consult_trade_last;              //协商成交最新价
		//std::int64_t click_trade_last;                //点击成交最新价
		//std::int64_t inquiry_trade_last;              //询价成交最新价
		//std::int64_t bid_trade_last;                  //竞买成交最新价

		std::int64_t reserved1;
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;
	};

	struct Snapshot_L2_Hkex
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		std::int64_t offer_price[10];
		std::int64_t offer_volume[10];
		std::int64_t offer_num_order[10];
		std::int64_t bid_price[10];
		std::int64_t bid_volume[10];
		std::int64_t bid_num_order[10];
		std::int64_t total_volume_trade;
		std::int64_t total_value_trade;
		std::int64_t yield_to_maturity;

		// 港股新增字段
		std::uint8_t suspension_indicator;          // 停牌标志
		std::uint8_t trading_session_status;		// 市场交易状态
		std::uint8_t trading_session_sub_id;		// 市场交易子状态
		char trading_session_control_flag;		    // 市场交易管控标志
		std::int64_t nominal_price;					// 名义价格
		std::int64_t reference_price;				// 集合竞价参考价格
		std::int64_t reference_upper_price;			// 集合竞价价格上限
		std::int64_t reference_lower_price;			// 集合竞价价格下限
		std::int64_t vcm_start_time;				// 波动率管控机制的开始时间
		std::int64_t vcm_end_time;					// 波动率管控机制的结束时间
		std::int64_t short_sell_shares_traded;		// 卖空量
		std::int64_t short_sell_turnover;			// 卖空额
		std::int64_t order_imbalance_quanty;		// 委托不平衡量
		std::int64_t indicative_equilibrium_price;	// 指示性平衡价格
		std::int64_t indicative_equilibrium_volume; // 指示性平衡量

		std::int64_t vcm_reference_price;           // vcm参考价格
		std::int64_t vcm_upper_price;               // vcm价格上限
		std::int64_t vcm_lower_price;               // vcm价格下限
		std::int64_t reserved1;
	};

	struct Snapshot_Main_Hkex
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;
		char tpc[8];		// 上海，MktStatus，对应MMDH的TradingSessionSubID; 深圳，TradingPhaseCode
		std::int64_t preclose;  // 对应MMDH的PreviousClosingPrice
		std::int64_t high;  // 对应MMDH的HighPrice
		std::int64_t low;   // 对应MMDH的LowPrice
		std::int64_t last;  // 对应MMDH的LastPrice
		std::int64_t close; // 收盘后，对应MMDH的NominalPrice
		std::int64_t offer_price[5];    // 对应MMDH的AskPrice1
		std::int64_t offer_volume[5];   // 对应MMDH的AggregateAskQuantity1
		std::int64_t bid_price[5];  // 对应MMDH的BidPrice1
		std::int64_t bid_volume[5]; // 对应MMDH的AggregateBidQuantity1
		std::int64_t total_volume_trade;    // 对应MMDH的SharesTraded
		std::int64_t total_value_trade; // 对应MMDH的Turnover
		std::int64_t nominal_price; // 对应MMDH的NominalPrice
		std::int64_t reference_price;   // 对应MMDH的ReferencePrice/VCMReferencePrice
		std::int64_t pos_upper_price;   // 对应MMDH的POSUpperPrice，深圳对应为卖盘上限xt
		std::int64_t pos_lower_price;   // 对应MMDH的POSLowerPrice，深圳对应为买盘下限xs
		std::int64_t reference_upper_price;   // 对应MMDH的UpperPrice，深圳对应为买盘上限xr
		std::int64_t reference_lower_price;		// 对应MMDH的LowerPrice，深圳对应为卖盘下限xu
		std::int64_t vcm_start_time;	// 对应MMDH的CoolingOffStartTime			
		std::int64_t vcm_end_time;		// 对应MMDH的CoolingOffEndTime	
		std::int64_t vcm_reference_price;   // 对应MMDH的VCMReferencePrice,深圳对应为买盘上限xi
		std::int64_t vcm_upper_price;   // 对应MMDH的VCMUpperPrice，深圳对应为买盘上限xe
		std::int64_t vcm_lower_price;		// 对应MMDH的VCMLowerPrice，深圳对应为卖盘下限xf
		std::uint8_t suspension_indicator;          // 仅港股通上海，SecTradingStatus，对应MMDH的SuspensionIndicator
		char order_imbalance_direction; // 仅港股通上海，OrdImbDirection，对应MMDH的OrderImbalanceDirection
		std::int64_t order_imbalance_quanty;    // 仅港股通上海，OrdImbQty，对应MMDH的OrderImbalanceQuantity

		std::int64_t reserved1;
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;
	};

	struct Snapshot_L1
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		char symbol[16];
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		char instrument_status[8];
		char tpc[8];
		std::int64_t offer_price[5];
		std::int64_t offer_volume[5];
		std::int64_t bid_price[5];
		std::int64_t bid_volume[5];
		std::int64_t num_trades;
		std::int64_t total_volume_trade;
		std::int64_t total_value_trade;
		std::int64_t high_limited;
		std::int64_t low_limited;
		std::int64_t total_offer_num;
		std::int64_t total_bid_num;
		std::int64_t iopv;
		std::int64_t nav;
		std::int64_t position;
		std::int64_t pratio1;
		std::int64_t pratio2;
		std::int64_t updown1;
		std::int64_t updown2;
		std::int64_t weighted_avg_price;
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;

	};

	struct Snapshot_L1_Bond
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t last_trade_type;                 // 债券最新价成交方式，1=匹配成交，2 = 协商成交，3 = 点击成交，4 = 询价成交，5 = 竞买成交
		std::int64_t close;
		char tpc[8];
		std::int64_t offer_price[5];
		std::int64_t offer_volume[5];
		std::int64_t bid_price[5];
		std::int64_t bid_volume[5];
		std::int64_t num_trades;
		std::int64_t total_volume_trade;
		std::int64_t total_value_trade;
		
		std::int64_t total_offer_quant;                 // 卖出总量
		std::int64_t total_bid_quant;                   // 买入总量
		std::int64_t weighted_avg_offer_price;
		std::int64_t weighted_avg_bid_price;
		
		std::int64_t updown1;
		std::int64_t updown2;
		std::int64_t weighted_avg_price;
		std::int64_t weighted_avg_rate;               // 深交所质押式回购的实时成交量加权平均利率加权平均利率
		std::int64_t weighted_avg_rate_updown;        // 质押式回购的加权平均利率涨跌BP
		std::int64_t preclose_weighted_average_rate;  // 昨收盘成交量加权平均利率
		std::int64_t auction_trade_last;        // 匹配成交最新成交价
		std::int64_t auction_trade_volume;            // 匹配成交成交量
		std::int64_t auction_trade_value;             // 匹配成交成交金额
		char sub_tpc[5][8];                          // 细分交易阶段代码

		//std::int64_t auction_trade_last;              //匹配成交最新价
		//std::int64_t consult_trade_last;              //协商成交最新价
		//std::int64_t click_trade_last;                //点击成交最新价
		//std::int64_t inquiry_trade_last;              //询价成交最新价
		//std::int64_t bid_trade_last;                  //竞买成交最新价

		std::int64_t reserved1;
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;
	};

	struct Snapshot_Index
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		std::int64_t total_num;
		std::int64_t total_volume;
		std::int64_t total_value;
	};

	struct Snapshot_Future
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		char symbol[16];
		char tpc[8];
		std::int32_t action_day;
		char settle_group_id[12];
		std::int32_t settle_id;
		std::int64_t preposition;
		std::int64_t preclose;
		std::int64_t presettle;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		std::int64_t settle;
		std::int64_t total_volume;
		std::int64_t total_value;
		std::int64_t total_position;
		std::int64_t high_limited;
		std::int64_t low_limited;
		std::int64_t predelta;
		std::int64_t current_delta;
		std::int64_t offer_price[5];
		std::int64_t offer_volume[5];
		std::int64_t bid_price[5];
		std::int64_t bid_volume[5];
		std::int64_t auction_price;
		std::int64_t auction_quant;
		std::int64_t avg_price;
		std::int64_t reserved1;      // 期权参考熔断价格
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;      
	};

	struct Snapshot_Sgex
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		char symbol[32];
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		std::int64_t last_match_quanty;            /*最新成交量*/
		std::int64_t total_volume;                 /*成交量*/
		std::int64_t total_value;                  /*成交额*/
		std::int64_t total_weight;                 /*成交重量*/
		std::int64_t init_open_interest;           /*初始持仓量*/
		std::int64_t open_interest;                /*持仓量*/
		std::int64_t interest_change;              /*持仓量变化*/
		std::int64_t settle;                       /*今结算价*/
		std::int64_t presettle;                    /*昨结算价*/
		std::int64_t life_low;                     /*历史最低价*/
		std::int64_t life_high;                    /*历史最高价*/
		std::int64_t high_limited;                 /*涨停板*/
		std::int64_t low_limited;                  /*跌停板*/
		std::int64_t avg_price;                    /*当日均价*/
		std::int64_t bid_one_price;                /*最高买*/
		std::int64_t bid_one_quanty;               /*申买量*/
		std::int64_t bid_one_imply_quanty;         /*申买推导量*/
		std::int64_t ask_one_price;                /*最低卖*/
		std::int64_t ask_one_quanty;               /*申卖量*/
		std::int64_t ask_one_imply_quanty;         /*申卖推导量*/
	};

	struct Trade
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t index;
		std::int64_t price;
		std::int64_t volume;
		char bsFlag;
		std::int64_t sellNo;
		std::int64_t buyNo;
		std::int16_t type;                          // 港股成交类型
		std::int32_t channel;
		std::int64_t biz_index;                     // sh biz index
	};

	struct Trade_Bond
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t index;
		std::int64_t price;
		std::int64_t volume;
		char bsFlag;
		std::int64_t sellNo;
		std::int64_t buyNo;
		std::int16_t type;                          // 港股成交类型
		std::int32_t channel;
		std::int64_t biz_index;                     // sh biz index

		std::uint8_t settlperiod;                //结算周期
		std::uint16_t settltype;                 //结算方式

		char secondaryorderid[16];               //竞买场次编号
		std::uint16_t bidexecinsttype;           //竞买成交方式
		std::int64_t marginprice;                //边际价格
		std::int16_t method_type;                          // 交易方式 1=匹配成交，2 = 协商成交，3 = 点击成交，4 = 询价成交，5 = 竞买成交
	};

	struct Order
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t index;
		std::int64_t price;
		std::int64_t volume;
		char side;
		char type;
		std::int32_t channel;
		std::int64_t origin_no;                    // for sh, 原始订单号
		std::int64_t biz_index;                    // for sh, biz index
	};

	struct Order_Bond
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t index;
		std::int64_t price;
		std::int64_t volume;
		char side;
		char type;
		std::int32_t channel;
		std::int64_t origin_no;                    // for sh, 原始订单号
		std::int64_t biz_index;                    // for sh, biz index

		// 协议交易
		char confirmid[8];
		char contactor[12];
		char contactinfo[30];

		std::uint16_t expirationdays;
		std::uint8_t expirationtype;

		// 债券现券交易业务报价及大额逐笔委托
		char quoteid[10];               //报价消息编号， 仅适用于点击成交
		char memberid[6];               //交易商代码
		char investortype[2];           //交易主体类型
		char investorid[10];            //交易主体代码
		char investorname[120];         //客户名称
		char tradercode[8];             //交易员代码
		std::uint8_t settlperiod;       //结算周期
		std::uint16_t settltype;       //结算方式
		char memo[160];                 //备注

		char secondaryorderid[16];       //竞买场次编号
		std::uint16_t bidtranstype;           //竞买业务类别
		std::uint16_t bidexecinsttype;        //竞买成交方式
		std::int64_t lowlimitprice;           //价格下限
		std::int64_t highlimitprice;          //价格上限
		std::int64_t minqty;                  //最低成交数量
		std::uint32_t tradedate;              //交易日期
		std::int16_t method_type;             // 交易方式 1=匹配成交，2 = 协商成交，3 = 点击成交，4 = 询价成交，5 = 竞买成交
	};

	struct Kline
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t numTrade;
		std::int64_t volume;
		std::int64_t value;
		std::int64_t position;
	};

	struct Snapshot_Nq
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		std::int64_t volume;
		std::int64_t value;
		std::int64_t position;
		std::int64_t high_limited;
		std::int64_t low_limited;
		std::int64_t offer_price[5];
		std::int64_t offer_volume[5];
		std::int64_t bid_price[5];
		std::int64_t bid_volume[5];
		std::int64_t reserved1;
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;
	};

	struct Snapshot_Nqzs
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::int8_t eq_tpc;                        // eqapi trading phase code
		std::int64_t offer_tz_price[10];            // tz = 投资
		std::int64_t offer_tz_volume[10];
		std::int32_t offer_tz_time[10];
		std::int64_t bid_tz_price[10];
		std::int64_t bid_tz_volume[10];
		std::int32_t bid_tz_time[10];
		std::int64_t offer_zs_price[10];            // zs = 做市
		std::int64_t offer_zs_volume[10];
		std::int32_t offer_zs_time[10];
		std::int64_t bid_zs_price[10];
		std::int64_t bid_zs_volume[10];
		std::int32_t bid_zs_time[10];
		std::int64_t reserved1;
		std::int64_t reserved2;
		std::int64_t reserved3;
		std::int64_t reserved4;
	};

	struct Snapshot_Cnsi
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		char symbol[16];
		std::int8_t source;
		std::int64_t preclose;
		std::int64_t open;
		std::int64_t high;
		std::int64_t low;
		std::int64_t last;
		std::int64_t close;
		std::int64_t updown;
		std::int64_t updown_rate;
		std::int64_t volume;
		std::int64_t value;
		double exchange_rate;
		std::int8_t currency;
		std::int64_t close_asia;
		std::int64_t close_europe;
		std::int64_t reserved1;
		std::int64_t reserved2;
	};

	struct Indic_MA
	{
		char security_id[16];
		std::uint16_t market;
		std::int32_t date;
		std::int32_t time;
		std::uint8_t quote_type;
		std::uint8_t sub_type;              // reserved
		std::int64_t ma_1;                  // 1 min
		std::int64_t ma_5;                  // 5 min
		std::int64_t ma_15;                 // 15 min
		std::int64_t ma_30;                 // 30 min
		std::int64_t ma_60;                 // 60 min
	};

#pragma pack()
}


#endif // __quote_data_h__
