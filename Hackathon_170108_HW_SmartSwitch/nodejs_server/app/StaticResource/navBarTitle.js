const navLists = [
    {
        title: "首页",
        path: "/",
        key: "home",
        icon: "desktop"
    }, {
        title: "品牌形象",
        path: "/bImages",
        key: "brandImages",
        icon: "smile-o"
    },
    {
        title: "最新资讯",
        path: "/lNews",
        key: "latestNews",
        icon: "message"
    },
    {
        title: "门店查询",
        path: "/aSearches",
        key: " addressSearches",
        icon: "environment-o"
    },

    {
        title: "积分兑换",
        path: "/cExchange",
        key: "creditsExchange",
        icon: "pay-circle-o"
    },
    {
        title: "会员专区",
        path: "/vip",
        key: " vipArea",
        icon: "team"
    }
];
const steps = [{
    title: "会员登录", description: "输入帐号和密码"
}, {title: "点击领取", description: "领取活动奖励"}, {
    title: "成功领取", description: "领取完毕"
}];

const gifts = [
    {
        type: "a",
        name: "iphone 7plus",
        num: 1
    }, {
        type: "b",
        name: "ipad mini4",
        num: 0
    }
    , {
        type: "c",
        name: "天堂伞",
        num: 1
    }
    , {
        type: "d",
        name: "10元购物券",
        num: 1
    }

];

export {navLists,steps,gifts}