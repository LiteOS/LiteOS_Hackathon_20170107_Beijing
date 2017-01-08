document.title="MMPLUS | 夏力维の留言板"
/**
 * 设备识别
 */

var device_now = "device_pc"

if (navigator.userAgent.indexOf('iPhone') !== -1) {
    device_now = "device_iphone"
}else if (navigator.userAgent.indexOf('iPad') !== -1) {
    device_now = "device_iphone"
} else if (navigator.userAgent.indexOf('Android') !== -1) {
    device_now = "device_android"

} else if (navigator.userAgent.indexOf('Macintosh') !== -1) {
    device_now = "device_mac"
}

var is_weixin = navigator.userAgent.indexOf('MicroMessenger') !== -1

var timeout_rooms_show_plus = (device_now === 'device_iphone') ? 200 : 100 // rooms 显示动画的时间


var members_json = {}
var last_comment_ids = {}
var rooms_info = {}

var initialRoomId = '0cd8429c1da249b6935d7eef72d7fc0b'
var initialRoomImg = "http://wx.qlogo.cn/mmopen/srPhFAq751Ih4TJNFR6af7w8BYEtSqqzqeMY7iaxtOAPgZGibt0Wpq5J5nrKqUEF05w4HibmT2UmGMR83UCSWXeHKX2F67zibhicN/0"
var initialRoomName = '夏力维の留言板'

/*
 * 初始化
 */
var w = window.innerWidth
var h = window.innerHeight
var r = 30
var minX = r - 5
var maxX = w - minX
var minY = r + 5
var maxY = h - minY
var x = maxX
var y = minY + 80
var baseZIndex = 11000
var MAXOFFSET = 30    // 触摸偏移的最大值 px
var MAXPERIOD = 150  // 触摸时间的最大值 ms

var touchId
var _clientX
var _clientY
var isStarted = false
var isMoved = false
var isExpanded = false

var _startX
var _startY
var _maxOffset = 0
var _time

var latestRoomIds = []
var targetRoomId = null
var MAXROOMS = 4


var $body = $(document.body)


var html =
    '<div class="im_chat_heads"></div>' +
    '<div class="im_logo transition" style="visibility:hidden;z-index:10000;background-image:url(http://www.hotpoor.org/static/img/mmplus_logo.png)"></div>' +
    '<div class="im_room_icon_arrow"></div>' +
    '<div class="im_menu ' + device_now + '" style="display:none;">' +
        '<div class="im_menu_toolbar clearfix">' +
          '<div class="im_menu_resize"></div>' +
          '<div class="im_menu_tabs_wrap">' +
            '<div class="im_menu_border" style="left:0%;"></div>' +
            '<div class="im_menu_tabs">' +
                '<div class="im_menu_tab im_menu_tab_home active" data-tab="home" data-offset="0"></div>' +
                '<div class="im_menu_tab im_menu_tab_order" data-tab="order" data-offset="1"></div>' +
                '<div class="im_menu_tab im_menu_tab_discover" data-tab="discover" data-offset="2"></div>' +
                '<div class="im_menu_tab im_menu_tab_me" data-tab="me" data-offset="3"></div>' +
            '</div>' +
           '</div>' +
         '</div>' +
        '<div class="im_menu_container">' +
            '<div class="im_menu_content" data-tab="home"></div>' +
            '<div class="im_menu_content" data-tab="order">order</div>' +
            '<div class="im_menu_content" data-tab="discover">discover</div>' +
            '<div class="im_menu_content" data-tab="me">me</div>' +
        '</div>' +
    '</div>' +
    '<div class="im_box ' + device_now + '" style="display:none;">' +
        '<div class="im_title">' +
            '<span class="im_chat_name"></span>' +
            '<span class="im_chat_icon_resize"></span>' +
        '</div>' +
        '<div class="im_rooms">' +
            '<div class="im_room_load_tip">消息加载中...</div>' +
        '</div>' +
        '<div class="im_edit_base">' +
            '<div class="im_edit_icon_plus"></div>' +
            '<div class="im_edit_input_text">' +
                '<textarea class="im_edit_content" placeholder="发消息..."></textarea>' +
            '</div>' +
            '<div class="im_edit_icon_send"></div>' +
        '</div>' +
        '<div class="im_edit_plus">' +
            '<div class="im_edit_toolbar">' +
                '<div class="im_edit_icon im_edit_icon_text" data-tool="text"></div>' +
                '<div class="im_edit_icon im_edit_icon_face" data-tool="face"></div>' +
                '<div class="im_edit_icon im_edit_icon_image" data-tool="image"></div>' +
                '<div class="im_edit_icon im_edit_icon_voice ' + (is_weixin ? 'device_wechat' : '') + '" data-tool="voice"></div>' +
                '<div class="im_edit_icon im_edit_icon_setting" data-tool="setting"></div>' +
            '</div>' +
            '<div class="im_edit_tool_container">' +
                '<div class="im_edit_tool im_edit_tool_intro" data-tool="intro">简介' +
                '</div>' +
                '<div class="im_edit_tool im_edit_tool_face" data-tool="face">' +
                    '<div class="tool_face_packs_wrap">' +
                        '<div class="tool_face_packs">' +
                            '<div class="tool_face_pack active" data-face-pack="facebook" style="background-image:url(http://img.plancats.com/face_fb_1.png?imageView2/1/w/100/h/100)"></div>' +
                            '<div class="tool_face_pack" data-face-pack="telegram" style="background-image:url(http://img.plancats.com/face_tg_1.png?imageView2/2/h/100)"></div>' +
                        '</div>' +
                        '<div class="tool_face_packs_border"></div>' +
                    '</div>' +
                    '<div class="tool_face_wrap">'

var html_face_items = '<div class="tool_face clearfix" data-face-pack="facebook" style="display: block;">'
for (var i = 1; i <= 32; i++) {
    var face_url = 'http://img.plancats.com/face_fb_' + i + '.png?imageView2/1/w/100/h/100'
    var face_id = 'face_fb_' + i
    html_face_items +=
        '<div class="tool_face_item" data-face-id="' + face_id + '" ' + 'data-face-url="' + face_url+ '" style="background-image:url(' + face_url + ')"></div>'
}
html_face_items += '</div>'
html += html_face_items

var html_face_items = '<div class="tool_face clearfix" data-face-pack="telegram">'
for (var i = 1; i <= 32; i++) {
    var face_url = 'http://img.plancats.com/face_tg_' + i + '.png?imageView2/2/h/100'
    var face_id = 'face_tg_' + i
    html_face_items +=
        '<div class="tool_face_item" data-face-id="' + face_id + '" ' + 'data-face-url="' + face_url  + '" style="background-image:url(' + face_url + ')"></div>'
}
html_face_items += '</div>'
html += html_face_items


html +=             '</div>' +
                '</div>' +
                '<div class="im_edit_tool im_edit_tool_image" data-tool="image" style="display: none;">' +
                    '<div class="tool_image_btns clearfix">' +
                        '<div class="tool_image_btn" data-name="photo" style="background-image:url(http://img.plancats.com/camera_gray.svg);">照片</div>' +
                        '<div class="tool_image_btn" data-name="panorama" style="background-image:url(http://img.plancats.com/panorama_gray.svg);">全景</div>' +
                    '</div>' +
                    '<div class="tool_image_upload" style="display: none;">' +
                        '<div class="upload_image_items">' +
                            //<div class="upload_image_item" data-index="1"><div class="upload_image_index">1/5</div><div class="upload_image_progress"><span class="upload_image_progress_circle">78%</span></div><div class="upload_image_cancel"></div></div class="upload_image_success"></div></div>
                        '</div>' +
                    '</div>' +
                '</div>' +
                '<div class="im_edit_tool im_edit_tool_voice ' + (is_weixin ? 'device_wechat' : '') + '" data-tool="voice">' +
                    '<button class="wx_record_mode_btn ' + device_now + ' active" id="wx_record_mode_single" style="left: 12px;bottom: 30px;">单次</button>' +
                    '<button class="wx_record_mode_btn ' + device_now + '" id="wx_record_mode_continue" style="left: 66px;bottom: 30px;">连续</button>' +
                    '<div class="wx_record_timer">60s</div>' +
                    '<div class="wx_record_control">' +
                        '<div class="wx_record_ring"></div>' +
                        '<div class="wx_record_start" id="wx_record_start" style="display:block;"></div>' +
                        '<div class="wx_record_stop" id="wx_record_stop"></div>' +
                        '<div class="wx_record_upload" id="wx_record_upload"></div>' +
                    '</div>' +
                    '<div class="wx_record_cancel_wrap">' +
                        '<span class="wx_record_cancel" id="wx_record_cancel"></span>' +
                        '<br><span class="wx_record_cancel_info">取消</span>'+
                    '</div>' +
                    '<div class="wx_record_info">开始录音</div>'+
                '</div>' +
                '<div class="im_edit_tool im_edit_tool_setting" data-tool="setting">' +
                    '<div class="tool_setting_volume">' +
                        '<div class="tool_setting_volume_icon"></div>' +
                        '<div class="tool_setting_volume_control">' +
                            '<div class="tool_setting_volume_bar">' +
                                '<div class="tool_setting_volume_progress" style="width: 100%;"></div>' +
                                '<div class="tool_setting_volume_dot" style="left: 100%;"></div>' +
                            '</div>' +
                        '</div>' +
                    '</div>' +
                '</div>' +
            '</div>' +
        '</div>' +
    '</div>' +
    '<div class="pop_card_cover">' +
        '<div class="pop_card_wrap">' +
            '<div class="pop_card"></div>' +
        '</div>' +
    '</div>'+
    '<div class="pop_card_cancel"></div>'+
    '<div style="display:none">'+
    '<input type="file" id="img_add_upload" multiple>'+
    '</div>'

$body.append(html)

var $chatHeads = $('.im_chat_heads')
var $logo = $('.im_logo')
var $imBox = $('.im_box')
var $baseInput = $('.im_edit_base')
var $cover = $('.cover')
var $rooms = $('.im_rooms')
var $baseInputEdit = $('.im_edit_input_text')
var $msgContent = $('.im_edit_content')
var $sendMsg = $('.im_edit_icon_send')
var $chatName = $('.im_chat_name')
var $chatResize = $('.im_chat_icon_resize')
var $roomArrow = $('.im_room_icon_arrow')

if (device_now === 'device_iphone') {
    $imBox.css({'height': (h - 80) + 'px'})
    $(".im_menu").css({'height': (h - 80) + 'px'})
    $(".style_help").remove()
    $("body").append('<div class="style_help"><style>.im_chat_head,.im_logo {top: initial;bottom: ' + (h - 30) + 'px;}</style></div>')
}


// 预设的一些置顶
var base_room_ids = [
    [initialRoomId,initialRoomImg,initialRoomName],
    ['783d11c69f7a465093244ac3634807b1', 'http://dn-shimo-image.qbox.me/aZUkor9Msm48ljgc/wangxin.jpeg', '王鑫の留言板'],
    ['64fc550233004b26907ff91cb2351654', 'https://dn-shimo-image.qbox.me/RhuKB34IuAUIusIp/image.jpeg', '罗楠の留言板'],
    ['985089e32a6e46d0b11217aab5c02301', 'http://dn-shimo-image.qbox.me/acYO3gVPiVgF8eQu/image.jpeg', '张同庆の留言板'],
    ['b7469aa980204664a9723cfda89b7714', 'http://wx.qlogo.cn/mmopen/kLns8yTqDLwD7LSiblyXHpC98D5ziaQFfu8W7sZ42ftmLpMZYdalyNOWQDSt0mWnl0gibjm0Ex8tfjZvDdeXFXHyg/0', '许志铭の留言板'],
    ['4e0cba4bd4cc4c3298ecf428c1e73519', 'http://wx.qlogo.cn/mmopen/viasnlibZqap4GwK7ua20icUVLcTas3GFia5lLleaYmTsgnO1Aj3icibMwY9UlSoeD1Smur9yUIPNSTbOWfuUYQvjm8TuXzgY7Wia54/0', 'Helen小布熊の留言板'],
]
var base_line, _i, _len;
base_line_div = document.createElement("div");
base_line_div.className = "im_room_items_top";
for (_i = 0, _len = base_room_ids.length; _i < _len; _i++) {
  base_line = base_room_ids[_i];
  $(base_line_div).append("<div class=\"im_room_item\" data-room-id=\"" + base_line[0] + "\">\n    <div class=\"im_room_item_headimg\" data-image=\""+base_line[1]+"\" style=\"background-image:url(" + base_line[1] + ")\"></div>\n    <div class=\"im_room_item_nickname\">" + base_line[2] + "</div>\n    <div class=\"im_room_item_latest_msg\">最近木有啥消息嘿</div>\n    <div class=\"im_room_item_latest_time\">刚刚</div>\n</div>");
}
$(".im_menu_content[data-tab='home']").append(base_line_div);

$("body").on("click",".im_room_item",function(e){
    isLogo = false
    $imMenu.hide()
    $imBox.show()
    var roomId = $(this).attr("data-room-id")
    if (latestRoomIds.indexOf(roomId) !== -1) {
        onClickSwitch(roomId)
    } else {
        targetRoomId = roomId
        createConversation($(this).attr("data-room-id"),$(this).children(".im_room_item_headimg").attr("data-image"),$(this).children(".im_room_item_nickname").text())
        
        // 选择 room
        $rooms.children().hide()
        $rooms.children('[data-room-id=' + targetRoomId + ']').show()
        $rooms.children('[data-room-id=' + targetRoomId + ']').on('scroll', onRoomScroll)

        $chatName.text(rooms_info[targetRoomId].roomName)
        var index = latestRoomIds.indexOf(targetRoomId)
        var offset = r * (2 * index + 1) + 5 * (index + 1)
        $roomArrow.css({ 'right': offset + 'px' })
    }
    var $currentRoom = $('.room[data-room-id=' + roomId + ']')
    var $currentRoomCard = $currentRoom.find('.room_card')
    $currentRoomCard[0].scrollIntoView(false)
    if ($currentRoomCard.height() <= $currentRoom.height() - 20) {
        if (last_comment_ids[roomId]) {
            $currentRoom.find('.room_load_btn').show()
        }
    }
})
targetRoomId = initialRoomId
createConversation(initialRoomId, initialRoomImg, initialRoomName)
/*
 * 初始化 ws 连接
 */
var ws = null
ws_type_noactions = ["LEAVE","RELOAD"]
function ws_run(){
    // ws = new WebSocket('ws://www.hotpoor.org:8036/ws?aim_id=' + initialRoomId)
    ws = new WebSocket('ws://www.hotpoor.org:8036/ws?aim_id=LITEOSDEMO')
    ws.onopen = function (e) {
        console.log('ws open')
        targetRoomId = initialRoomId
        createConversation(initialRoomId, initialRoomImg, initialRoomName)
        //createConversation('b3380cc1020e42ce9ca6df049cc94ee3', 'http://dn-shimo-image.qbox.me/VmwjE08b1d4gKTZ0/608282405.jpg', '铁掌柜の留言板')
        targetRoomId = "783d11c69f7a465093244ac3634807b1"
        createConversation('783d11c69f7a465093244ac3634807b1', 'http://dn-shimo-image.qbox.me/aZUkor9Msm48ljgc/wangxin.jpeg', '王鑫の留言板')
        targetRoomId = "64fc550233004b26907ff91cb2351654"
        createConversation('64fc550233004b26907ff91cb2351654', 'https://dn-shimo-image.qbox.me/RhuKB34IuAUIusIp/image.jpeg', '罗楠の留言板')
        targetRoomId = "985089e32a6e46d0b11217aab5c02301"
        createConversation('985089e32a6e46d0b11217aab5c02301', 'http://dn-shimo-image.qbox.me/acYO3gVPiVgF8eQu/image.jpeg', '张同庆の留言板')
        targetRoomId = "b7469aa980204664a9723cfda89b7714"
        createConversation('b7469aa980204664a9723cfda89b7714', 'http://wx.qlogo.cn/mmopen/kLns8yTqDLwD7LSiblyXHpC98D5ziaQFfu8W7sZ42ftmLpMZYdalyNOWQDSt0mWnl0gibjm0Ex8tfjZvDdeXFXHyg/0', '许志铭の留言板')
        targetRoomId = "4e0cba4bd4cc4c3298ecf428c1e73519"
        createConversation('4e0cba4bd4cc4c3298ecf428c1e73519', 'http://wx.qlogo.cn/mmopen/viasnlibZqap4GwK7ua20icUVLcTas3GFia5lLleaYmTsgnO1Aj3icibMwY9UlSoeD1Smur9yUIPNSTbOWfuUYQvjm8TuXzgY7Wia54/0', 'Helen小布熊の留言板')
        // 选择 room
        $rooms.children().hide()
        $rooms.children('[data-room-id=' + targetRoomId + ']').show()
        $rooms.children('[data-room-id=' + targetRoomId + ']').on('scroll', onRoomScroll)

        $chatName.text(rooms_info[targetRoomId].roomName)
        var index = latestRoomIds.indexOf(targetRoomId)
        var offset = r * (2 * index + 1) + 5 * (index + 1)
        $roomArrow.css({ 'right': offset + 'px' })
    }
    ws.onmessage = function (e) {
        console.log('ws message', e.data)
        var msg = JSON.parse(e.data)
        if (ws_type_noactions.indexOf(msg[0]) == -1) {
            addMessage(msg)
        }else{
            if(msg[0]=="RELOAD"){
                window.location.href= window.location.href+"&time="+(Math.random()*100)
            }
        }
    }
    ws.onclose = function (e) {
        console.log('ws close')
        ws_run()
    }
    ws.onerror = function (e) {
        console.log('ws error')
    }
}
ws_run()

function joinMoreRooms (roomIds) {
    var msg = ['JOINMOREROOMS', {}, initialRoomId, roomIds]
    ws.send(JSON.stringify(msg))
}




function addMessage (msg) {
    var msgType = msg[0]
    var roomId = msg[2]
    var content = msg[1].content
    content = escapeHTML(content)
    content = content.replace(/\n/g, '<br>')
    var headimg = msg[1].headimgurl
    var nickname = msg[1].nickname
    var timer = msg[1].time
    var time = formatDate(timer)
    var user_id = msg[1].user_id
    var tel = msg[1].tel
    var comment_id = msg[1].comment_id
    var comment_sequence = msg[1].sequence

    console.log(content)
    var content_type = content.split("//")[0]
    var content_values = content.split("//")[1]

    var msg_owner = (user_id === USER_ID) ? 'msg_self' : 'msg_other'

    var msg_headimg_hide = ""
    var msg_nickname_hide = ""
    if (rooms_info[roomId] == undefined){
        rooms_info[roomId] = {
            "imgUrl": "",
            "roomName": "",
            "createtime": timer,
            "finishtime": 0,
            "room_time_flag": timer,
            "createuser":"",
            "finishuser":"",
            "createcommentsequence":"",
            "finishcommentsequence":"",
        }
    }
    if(timer - rooms_info[roomId].finishtime <= 300){
        var msg_time_hide = "msg_time_hide"
        if (rooms_info[roomId].finishuser == user_id) {
            msg_headimg_hide = "msg_headimg_hide"
            msg_nickname_hide = "msg_nickname_hide"
        }
    }else{
        var msg_time_hide = ""
    }

    rooms_info[roomId].finishtime = timer
    rooms_info[roomId].finishuser = user_id
    rooms_info[roomId].finishcommentsequence = comment_id+"_"+comment_sequence
    var msg_html =
        '<div class="msg_time '+msg_time_hide+'" data-time="'+timer+'">' + time + '</div>' +
        '<div data-comment-flag="' + comment_id + '_' + comment_sequence + '" class="msg ' + msg_owner + ' ' + device_now + '">' +
            '<div class="msg_headimg '+msg_headimg_hide+'" style="background-image:url(' + headimg + ')"></div>' +
            '<div class="msg_nickname '+msg_nickname_hide+'">' + nickname + '</div>' +
            '<div class="msg_content_wrapper">' +
                '#{msg_content_html}' + // 不同 msgType 有不同的 HTML 片段
            '</div>' +
        '</div>'
    var msg_content_html = ''
    var error_img = "/static/img/tools/error_img_"+parseInt(Math.random()*10%2)+".png"
    // 评论 COMMENT
    if (msgType === 'COMMENT') {
        content_values = content.split(content_type+"//")[1]
        if (content_type === 'HWEBFACEIMG') {
            var face_url = decodeURIComponent(content_values)
            msg_content_html =
                '<span class="msg_content face">' +
                    '<div class="msg_face_wrap" data-face-url="' + face_url + '">' +
                        '<img crossorigin="Anonymous" class="msg_face" src="' + face_url + '"  onerror="this.src=\''+error_img+'\'">' +
                    '</div>' +
                '</span>'
        } else if (content_type === 'HWEBIMG') {
            // 外链图片
            console.log(content_values)
            var url = decodeURIComponent(content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_image">' +
                    '<div class="msg_image_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img crossorigin="Anonymous" class="msg_image_thumbnail" src="' + thumbnail_url + '">' +
                    '</div>' +
                '</span>'
            
        } else if (content_type === 'HWEBPANORAMA') {
            // 外链全景图片
            console.log(content_values)
            var url = decodeURIComponent(content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_panorama">' +
                    '<div class="msg_panorama_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img crossorigin="Anonymous" class="msg_panorama_thumbnail" src="' + thumbnail_url + '">' +
                    '</div>' +
                '</span>'
        } else if (content_type === 'HQWEBIMG') {
            // 图片
            console.log(content_values)
            var url = decodeURIComponent("http://image.hotpoor.org/"+roomId+"_"+content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_image">' +
                    '<div class="msg_image_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img crossorigin="Anonymous" class="msg_image_thumbnail" src="' + thumbnail_url + '?imageView2/2/w/300">' +
                    '</div>' +
                '</span>'
        } else if (content_type === 'HQWEBPANORAMA') {
            console.log(content_values)
            var url = decodeURIComponent("http://image.hotpoor.org/"+roomId+"_"+content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_panorama">' +
                    '<div class="msg_panorama_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img crossorigin="Anonymous" class="msg_panorama_thumbnail" src="' + thumbnail_url + '?imageView2/2/w/300">' +
                    '</div>' +
                '</span>'
        } else {
            msg_content_html = '<span class="msg_content">' + content + '</span>'
        }
        msg_html = msg_html.replace('#{msg_content_html}', msg_content_html)
        $rooms.children('[data-room-id=' + roomId + ']').children(".room_card").append(msg_html)
    
    // 语音 AUDIO
    } else if (msgType === 'AUDIO') {
        if (content_type === 'HQAUDIO') {
            var audio_src = "http://audio.hotpoor.org/" + roomId + "_" + user_id + "_" + content_values.split(",")[0]
            var audio_length = parseInt(content_values.split(",")[1]) + "\'\'"
            var audio_media_id = content_values.split(",")[2]
            $(".msg_self.audio_wait[data-media='"+audio_media_id+"']").fadeOut(500,function(){
                $(".msg_self.audio_wait[data-media='"+audio_media_id+"']").remove()
            })
        } else {
            var audio_src = ""
            var audio_length = "错误"
        }

        msg_content_html = 
            '<span class="msg_content audio" data-type="audio" data-audio-src="' + audio_src + '">' +
                '<div class="msg_audio">' +
                    '<span class="msg_audio_btn"></span>' +
                    '<span class="msg_audio_line"></span>' +
                    '<span class="msg_audio_time">' + audio_length + '</span>' +
                '</div>' +
                '<div class="msg_audio_cover"></div>' +
            '</span>'
        msg_html = msg_html.replace('#{msg_content_html}', msg_content_html)
        $rooms.children('[data-room-id=' + roomId + ']').children(".room_card").append(msg_html)

    // 语音转文字 AUDIOCAPTION
    } else if (msgType === 'AUDIOCAPTION') {
        content = content.split("百度语音转文字: ")[1]
        var audiocaption_logo = "http://img.plancats.com/baidu_speech.png"
        var msg_audio_caption_html =
            '<br>' +
            '<span class="msg_content audiocaption" style="font-style:italic;">' + content +
                '<br>' +
                '<div style="margin-top: 4px;height: 20px;padding-top: 5px;box-shadow: 0px -1px 0px rgba(255,255,255,0.6);">' +
                    '<img src="' + audiocaption_logo + '" style="height: 16px;border-radius: 2px;">' +
                '</div>' +
            '</span>'
        $('[data-comment-flag="' + comment_id + '_' + comment_sequence + '"]').find('.msg_content_wrapper').append(msg_audio_caption_html)
    } else if (msgType === 'IOTDATA'){
        msg_content_html = '<span class="msg_content">' + content + '</span>'
        msg_html = msg_html.replace('#{msg_content_html}', msg_content_html)
        $rooms.children('[data-room-id=' + roomId + ']').children(".room_card").append(msg_html)
        return
    } else if (msgType === 'LITEOSDEMO') {
        msg_content_html = '<span class="msg_content">' + content + '</span>'
        msg_html = msg_html.replace('#{msg_content_html}', msg_content_html)
        $rooms.children('[data-room-id=' + roomId + ']').children(".room_card").append(msg_html)
        return
    } else {
        return
    }


    // 聊天记录滚动到最底部
    if (msg_owner == "msg_self"){
        if($("[data-comment-flag='"+comment_id+"_"+comment_sequence+"']").length>=1){
            $('.room[data-room-id=' + roomId + ']').stop().animate({"scrollTop":$("[data-comment-flag='"+comment_id+"_"+comment_sequence+"']")[0].offsetTop})
        }
    }

    members_json[user_id] = {"headimgurl":headimg,"nickname":nickname,"tel": tel}

    if (latestRoomIds.indexOf(roomId) === -1) {
        if (rooms_info[roomId]!=undefined){
            createConversation(roomId, rooms_info[roomId].imgUrl, rooms_info[roomId].roomName)
        }else{
            createConversation(roomId, "", "")
        }
    }
    if (isExpanded) {
        onMessageExpanded(roomId, msg)
    } else {
        onMessageCollapsed(roomId, msg)
    }

    rooms_info[roomId]["latestComment"] = msg
    item_text = ""
    if(msg[0]=="AUDIO"){
        item_text = "[语音]"+msg[1].plus.content
    }else if(msg[0]=="COMMENT"){
        if(msg[1].content.indexOf("HQWEBIMG//")>=0||msg[1].content.indexOf("HWEBIMG//")>=0){
            item_text = "[图片]"
        }else if(msg[1].content.indexOf("HQWEBPANORAMA//")>=0||msg[1].content.indexOf("HWEBPANORAMA//")>=0){
            item_text = "[全景图片]"
        }else{
            item_text = msg[1].content
        }
    }
    item_text = msg[1].nickname +": "+item_text
    $(".im_room_item[data-room-id='"+roomId+"']>.im_room_item_latest_msg").text(item_text)
    $(".im_room_item[data-room-id='"+roomId+"']>.im_room_item_latest_time").text(formatDate(msg[1].time))
}


function loadMessage (msg) {
    var msgType = msg[0]
    var roomId = msg[2]
    var content = msg[1].content
    content = escapeHTML(content)
    content = content.replace(/\n/g, '<br>')
    var headimg = msg[1].headimgurl
    var nickname = msg[1].nickname
    var timer = msg[1].time
    var time = formatDate(timer)
    var user_id = msg[1].user_id
    var tel = msg[1].tel
    var plus = msg[1].plus
    plus_content = escapeHTML(plus.content)
    plus_type = escapeHTML(plus.type)
    if (plus_type == "百度语音转文字"){
        plus_content = plus_content.split("百度语音转文字: ")[1]
    }
    var comment_id = msg[1].comment_id
    var comment_sequence = msg[1].sequence

    var content_type = content.split("//")[0]
    var content_values = content.split("//")[1]

    var msg_owner = (user_id === USER_ID) ? 'msg_self' : 'msg_other'


    var msg_headimg_hide = ""
    var msg_nickname_hide = ""
    
    if(rooms_info[roomId].createtime - timer <= 300){
        var msg_time_hide = "msg_time_hide"
        msg_headimg_hide = "msg_headimg_hide"
        msg_nickname_hide = "msg_nickname_hide"
    }else{
        var msg_time_hide = ""   
    }
    if (rooms_info[roomId].createuser != user_id) {
        $(".msg[data-comment-flag='"+rooms_info[roomId].createcommentsequence+"']>.msg_headimg").removeClass("msg_headimg_hide")
        $(".msg[data-comment-flag='"+rooms_info[roomId].createcommentsequence+"']>.msg_nickname_hide").removeClass("msg_nickname_hide")
    }

    rooms_info[roomId].createtime = timer
    rooms_info[roomId].createuser = user_id
    rooms_info[roomId].createcommentsequence = comment_id+"_"+comment_sequence
    var msg_html =
        '<div class="msg_time '+msg_time_hide+'" data-time="'+timer+'">' + time + '</div>' +
        '<div data-comment-flag="' + comment_id + '_' + comment_sequence + '" class="msg ' + msg_owner + ' ' + device_now + '">' +
            '<div class="msg_headimg '+msg_headimg_hide+'" style="background-image:url(' + headimg + ')"></div>' +
            '<div class="msg_nickname '+msg_nickname_hide+'">' + nickname + '</div>' +
            '<div class="msg_content_wrapper">' +
                '#{msg_content_html}' + // 不同 msgType 有不同的 HTML 片段
            '</div>' +
        '</div>'
    var msg_content_html = ''
    var error_img = "/static/img/tools/error_img_"+parseInt(Math.random()*10%2)+".png"
    if (msgType === 'COMMENT') {
        content_values = content.split(content_type+"//")[1]
        if (content_type === 'HWEBFACEIMG') {
            var face_url = decodeURIComponent(content_values)
            msg_content_html =
                '<span class="msg_content face">' +
                    '<div class="msg_face_wrap" data-face-url="' + face_url + '">' +
                        '<img class="msg_face" src="' + face_url + '"  onerror="this.src=\''+error_img+'\'">' +
                    '</div>' +
                '</span>'
        } else if (content_type === 'HWEBIMG') {
            // 外链图片
            console.log(content_values)
            var url = decodeURIComponent(content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_image">' +
                    '<div class="msg_image_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img class="msg_image_thumbnail" src="' + thumbnail_url + '">' +
                    '</div>' +
                '</span>'
            
        } else if (content_type === 'HWEBPANORAMA') {
            // 外链全景图片
            console.log(content_values)
            var url = decodeURIComponent(content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_panorama">' +
                    '<div class="msg_panorama_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img class="msg_panorama_thumbnail" src="' + thumbnail_url + '">' +
                    '</div>' +
                '</span>'
        } else if (content_type === 'HQWEBIMG') {
            // 图片
            console.log(content_values)
            var url = decodeURIComponent("http://image.hotpoor.org/"+roomId+"_"+content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_image">' +
                    '<div class="msg_image_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img class="msg_image_thumbnail" src="' + thumbnail_url + '?imageView2/2/w/300">' +
                    '</div>' +
                '</span>'
        } else if (content_type === 'HQWEBPANORAMA') {
            console.log(content_values)
            var url = decodeURIComponent("http://image.hotpoor.org/"+roomId+"_"+content_values)
            console.log(url)
            var thumbnail_url = url+""
            msg_content_html = 
                '<span class="msg_content msg_panorama">' +
                    '<div class="msg_panorama_thumbnail_wrap clearfix" data-url="' + url + '">' +
                        '<img crossorigin="Anonymous" class="msg_panorama_thumbnail" src="' + thumbnail_url + '?imageView2/2/w/300">' +
                    '</div>' +
                '</span>'
        } else {
            msg_content_html = '<span class="msg_content">' + content + '</span>'
        }


        msg_html = msg_html.replace('#{msg_content_html}', msg_content_html)
    
    } else if (msgType === 'AUDIO') {
        if (content_type === 'HQAUDIO') {
            var audio_src = "http://audio.hotpoor.org/" + roomId + "_" + user_id + "_" + content_values.split(",")[0]
            var audio_length = parseInt(content_values.split(",")[1]) + "\'\'"
        } else {
            var audio_src = ""
            var audio_length = "错误"
        }
        var audiocaption_logo = "http://img.plancats.com/baidu_speech.png"
        msg_content_html = 
            '<span class="msg_content audio" data-type="audio" data-audio-src="' + audio_src + '">' +
                '<div class="msg_audio">' +
                    '<span class="msg_audio_btn"></span>' +
                    '<span class="msg_audio_line"></span>' +
                    '<span class="msg_audio_time">' + audio_length + '</span>' +
                '</div>' +
                '<div class="msg_audio_cover"></div>' +
            '</span>' +
            '<br>' +
                '<span class="msg_content audiocaption" style="font-style:italic;">' + plus_content +
                    '<br>' +
                    '<div style="margin-top: 4px;height: 20px;padding-top: 5px;box-shadow: 0px -1px 0px rgba(255,255,255,0.6);">' +
                        '<img src="' + audiocaption_logo + '" style="height: 16px;border-radius: 2px;">' +
                    '</div>' +
                '</span>'

    
        msg_html = msg_html.replace('#{msg_content_html}', msg_content_html)
    }
    $rooms.children('[data-room-id=' + roomId + ']').children(".room_card").prepend(msg_html)
}

// 发送消息
function submitComment(content) {
    $.ajax({
        url: 'http://www.hotpoor.org/api/comment/submit',
        type: 'POST',
        dataType: 'json',
        data: {
            app: WX_APP,
            aim_id: targetRoomId,
            content: content
        },
        success: function (data) {
            console.log(data)
        },
        error: function (error) {
            console.log(error)
        }
    })
}
function secretSubmitComment(content,aim_id) {
    $.ajax({
        url: 'http://www.hotpoor.org/api/comment/submit_secret',
        type: 'POST',
        dataType: 'json',
        data: {
            app: WX_APP,
            aim_id: aim_id,
            content: content,
            msgtype: aim_id
        },
        success: function (data) {
            console.log(data)
        },
        error: function (error) {
            console.log(error)
        }
    })
}



/*
 * 事件监听
 */

if (window.navigator.userAgent.indexOf('Mobile') !== -1) {
    $chatHeads.on('touchstart', function (e) {
        // console.log('touch start')
        e.stopPropagation()
        e.preventDefault()
        isStarted = true
        isMoved = false
        _maxOffset = 0
        _time = e.timeStamp
        var touch = e.originalEvent.targetTouches[0]
        touchId = touch.identifier
        _clientX = touch.clientX
        _clientY = touch.clientY
        _startX = _clientX
        _startY = _clientY
    })

    $(window).on('touchmove', function (e) {
        // console.log('touch move')
        if (isStarted) {
            e.preventDefault()
            isMoved = true;
            if (!isExpanded) {
                var els = $chatHeads.children()
                for (var i = 0,len = els.length; i < len; i++) {
                    els[i].classList.remove('transition')
                }

                var touch = e.originalEvent.targetTouches[0]
                if (touch.identifier === touchId) {
                    var clientX = touch.clientX
                    var clientY = touch.clientY
                    if (Math.abs(clientX - _startX) > _maxOffset) {
                        _maxOffset = Math.abs(clientX - _startX)
                    }
                    if (Math.abs(clientY - _startY) > _maxOffset) {
                        _maxOffset = Math.abs(clientY - _startY)
                    }
                    var dx = clientX - _clientX
                    var dy = clientY - _clientY
                    _clientX = clientX
                    _clientY = clientY
                    var _x = x + dx
                    var _y = y + dy
                    if (_y < minY) {
                        _y = minY
                        onMoveEnd(_x, _y)
                        isStarted = false
                    } else if (_y > maxY) {
                        _y = maxY
                        onMoveEnd(_x, _y)
                        isStarted = false
                    } else {
                        // console.log('max offset', _maxOffset)
                        if (_maxOffset > MAXOFFSET) {
                            move(_x, _y)
                        } else {
                            moveTogether(_x, _y)
                        }
                    }
                }
            } else {
                var touch = e.originalEvent.targetTouches[0]
                var clientX = touch.clientX
                var clientY = touch.clientY
                if (Math.abs(clientX - _startX) > _maxOffset) {
                    _maxOffset = Math.abs(clientX - _startX)
                }
                if (Math.abs(clientY - _startY) > _maxOffset) {
                    _maxOffset = Math.abs(clientY - _startY)
                }
            }
        }
    })

    $(window).on('touchend', function (e) {
        // console.log('touch end')
        if (isStarted) {
            isStarted = false
            var roomId = e.target.dataset.roomId

           var _isMoved = true
           if(device_now == "device_pc"){
                if(isMoved){
                    if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                       var _isMoved = false
                    }
                }
           }else if (device_now == "device_iphone"){
                if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                   var _isMoved = false
                }
           }else if (device_now == "device_android"){
                if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                   var _isMoved = false
                }
           }else if (device_now == "device_mac"){
                if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                   var _isMoved = false
                }
           }


            //var _isMoved = isMoved
            //if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
              //  _isMoved = false
            //}
            //if (!isMoved && (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET)) {
               //$("body").prepend(e.timeStamp - _time + " - " + _maxOffset +"<br>")
               // _isMoved = false
           // } else {
               // _isMoved = true
            //}

            if (!isExpanded) {
                if (!_isMoved) {
                    onClickCollapsed()
                } else {
                    onMoveEnd()
                }
            } else {
                if (!_isMoved) {
                    if (isLogo) {
                        isLogo = false
                        $imMenu.hide()
                        $imBox.show()
                        onClickSwitch(roomId)
                    } else {
                        if (roomId === targetRoomId) {
                            onClickExpanded()
                        } else {
                            onClickSwitch(roomId)
                        }
                    }
                    // $('.room[data-room-id=' + roomId + ']>.room_card')[0].scrollIntoView(false)
                    var $currentRoom = $('.room[data-room-id=' + roomId + ']')
                    var $currentRoomCard = $currentRoom.find('.room_card')
                    $currentRoomCard[0].scrollIntoView(false)
                    if ($currentRoomCard.height() <= $currentRoom.height() - 20) {
                        if (last_comment_ids[roomId]) {
                            $currentRoom.find('.room_load_btn').show()
                        }
                    }
                }
            }
        }
    })
} else {
    $chatHeads.on('mousedown', function (e) {
        e.stopPropagation()
        e.preventDefault()
        isStarted = true
        isMoved = false
        _maxOffset = 0
        _time = e.timeStamp
        _clientX = e.clientX
        _clientY = e.clientY
        _startX = _clientX
        _startY = _clientY
    })

    var isMousemoveArr = []
    $(window).on('mousemove', function (e) {
        if (isStarted) {
            e.preventDefault()
            isMoved = true
            console.log("mousemove")
            if (!isExpanded) {
                var els = $chatHeads.children()
                for (var i = 0,len = els.length; i < len; i++) {
                    els[i].classList.remove('transition')
                }
                var clientX = e.clientX
                var clientY = e.clientY
                var dx = clientX - _clientX
                var dy = clientY - _clientY
                _clientX = clientX
                _clientY = clientY
                var _x = x + dx
                var _y = y + dy
                if (Math.abs(clientX - _startX) > _maxOffset) {
                        _maxOffset = Math.abs(clientX - _startX)
                }
                if (Math.abs(clientY - _startY) > _maxOffset) {
                        _maxOffset = Math.abs(clientY - _startY)
                }
console.log(dx, dy)
                //isMousemoveArr.push([dx, dy])
                if (_y < minY) {
                    _y = minY
                    onMoveEnd(_x, _y)
                    isStarted = false
                } else if (_y > maxY) {
                    _y = maxY
                    onMoveEnd(_x, _y)
                    isStarted = false
                } else {
                    if (_maxOffset > MAXOFFSET) {
                        move(_x, _y)
                    } else {
                        moveTogether(_x, _y)
                    }
                }
            } else {
                var clientX = e.clientX
                var clientY = e.clientY
                if (Math.abs(clientX - _startX) > _maxOffset) {
                    _maxOffset = Math.abs(clientX - _startX)
                }
                if (Math.abs(clientY - _startY) > _maxOffset) {
                    _maxOffset = Math.abs(clientY - _startY)
                }
            }
        }
    })

    $(window).on('mouseup', function (e) {
        if (isStarted) {
                     console.log(isStarted)
            isStarted = false
            var roomId = e.target.dataset.roomId

            var _isMoved = true
           if(device_now == "device_pc"){
                if(isMoved){
                    if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                       var _isMoved = false
                    }
                }
           }else if (device_now == "device_iphone"){
                if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                   var _isMoved = false
                }
           }else if (device_now == "device_android"){
                if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                   var _isMoved = false
                }
           }else if (device_now == "device_mac"){
                if (e.timeStamp - _time < MAXPERIOD && _maxOffset < MAXOFFSET) {
                   var _isMoved = false
                }
           }
           //$("body").prepend(e.timeStamp - _time + " - " + _maxOffset +"<br>")

            if (!isExpanded) {
                if (_isMoved) {
console.log('onMoveEnd')
                    onMoveEnd()
                } else {
console.log('onClickCollapsed')
                    onClickCollapsed()
                }
            } else {
                if (!_isMoved) {
                    if (isLogo) {
                        isLogo = false
                        $imMenu.hide()
                        $imBox.show()
                        onClickSwitch(roomId)
                    } else {
                        if (roomId === targetRoomId) {
                            onClickExpanded()
                        } else {
                            onClickSwitch(roomId)
                        }
                    }
                    // $('.room[data-room-id=' + roomId + ']>.room_card')[0].scrollIntoView(false)
                    var $currentRoom = $('.room[data-room-id=' + roomId + ']')
                    var $currentRoomCard = $currentRoom.find('.room_card')
                    $currentRoomCard[0].scrollIntoView(false)
                    if ($currentRoomCard.height() <= $currentRoom.height() - 20) {
                        if (last_comment_ids[roomId]) {
                            $currentRoom.find('.room_load_btn').show()
                        }
                    }
                }
            }
        }
    })
}


var $imMenu = $('.im_menu')
var isLogo = false
$logo.on('click', function (e) {
    e.stopPropagation()
    if (isExpanded) {
        if (!isLogo) {
            isLogo = true
            $imBox.hide()
            $imMenu.show()
            var index = latestRoomIds.length
            var offset = r * (2 * index + 1) + 5 * (index + 1)
            $roomArrow.css({ 'right': offset + 'px' })
        } else {
            isLogo = false
            $imMenu.hide()
            onClickExpanded()
        }
    }
})

var im_menu_border_timeout = null
$("body").on("click", ".im_menu_tab", function (e) {
    var el = e.currentTarget
    var tab = el.dataset.tab
    var index = el.dataset.offset
    $('.im_menu_content').hide()
    $('.im_menu_content[data-tab=' + tab + ']').show()

    $('.im_menu_tab').removeClass('active')
    $('.im_menu_tab[data-tab=' + tab + ']').addClass('active')
    var value = index * 25 + '%'
    $('.im_menu_border').stop()

    if (device_now === 'device_android') {
        clearTimeout(im_menu_border_timeout)
        im_menu_border_timeout = setTimeout(function () {
            $('.im_menu_border').animate({'left': value},250)
        }, 100)
    } else {
        $('.im_menu_border').animate({'left': value},250)
    }
})


$(window).on('resize', function (e) {
    // console.log('resize')
    var old_h = h
    w = window.innerWidth
    h = window.innerHeight
    maxX = w - minX
    maxY = h - minY
    x = maxX
    y = y / old_h * h
    resetStyle()
    if (device_now === 'device_iphone') {
        $imBox.css({'height': (h - 80) + 'px'})
        $(".style_help").remove()
        $("body").append('<div class="style_help"><style>.im_chat_head,.im_logo {top: initial;bottom: ' + (h - 30) + 'px;}</style></div>')
    }
})

$imBox.on('animationend', function (e) {
    $imBox.removeClass('expand')
})
$imBox.on('webkitAnimationEnd', function (e) {
    $imBox.removeClass('expand')
})
$logo.on('transitionend', function (e) {
    if (!isExpanded) {
        $logo.css({'visibility': 'hidden'})
    }
})
$chatResize.on('click', function (e) {
    if ($chatResize.hasClass('im_chat_icon_resize_full')) {
        $chatResize.removeClass('im_chat_icon_resize_full')
        $imBox.removeClass('im_box_full')
    } else {
        $chatResize.addClass('im_chat_icon_resize_full')
        $imBox.addClass('im_box_full')
    }
})
var $imMenuResize = $('.im_menu_resize')
$("body").on("click", ".im_menu_resize", function (e) {
    if ($imMenuResize.hasClass('im_menu_resize_full')) {
        $imMenuResize.removeClass('im_menu_resize_full')
        $imMenu.removeClass('im_menu_full')
    } else {
        $imMenuResize.addClass('im_menu_resize_full')
        $imMenu.addClass('im_menu_full')
    }
})
$sendMsg.on('click', function (e) {
    var content = $msgContent.val()
    if (content.replace(/\s/g, '') !== '') {
        submitComment(content)
        $msgContent.val('')
        $sendMsg.removeClass('im_edit_icon_send_active')
        adjustInputHeight()
    }
})
$msgContent.on('input', function (e) {
    adjustInputHeight()
    var content = $msgContent.val()
    if (content.replace(/\s/g, '') !== '') {
        $sendMsg.addClass('im_edit_icon_send_active')
    } else {
        $sendMsg.removeClass('im_edit_icon_send_active')
    }
})
$baseInputEdit.on('click', function (e) {
    $msgContent.focus()
})

$baseInputEdit.on('keydown', function(e) {
    if( e.ctrlKey && e.key == "Enter"){
        $(".im_edit_icon_send_active").click()
    }
})


function adjustInputHeight () {
    $baseInput.removeClass('row-1 row-2 row-3')
    var scrollHeight = $msgContent[0].scrollHeight
    if (scrollHeight === 20) {
        $baseInput.addClass('row-1')
    } else if (scrollHeight === 40) {
        $baseInput.addClass('row-2')
    } else {
        $baseInput.addClass('row-3')
    }
}


$('.im_edit_icon_plus').on('click', function () {
    $msgContent.blur()
    showEditPlus()
})
$('.im_edit_icon_text').on('click', function () {
    hideEditPlus()
})


var $imEdit = $('.im_edit')
var $imEditBase = $('.im_edit_base')
var $imEditPlus = $('.im_edit_plus')
var $imEditIcons = $('.im_edit_icon')
var $imEditTools = $('.im_edit_tool')


function showEditPlus () {
    $imEditBase.hide()
    if (device_now === 'device_android') {
        $imEditPlus.css({'transform': 'translate3d(0,-180px,0)', 'transition': 'none'}).fadeIn()
    } else {
        $imEditPlus.show().addClass('plus')
    }

    setTimeout(function () {
        $imBox.css({'padding-bottom': '230px'})
    }, 500)

    $('.im_edit_icon_face').click()
}

function hideEditPlus () {
    $imBox.css({'padding-bottom': '50px'})

    if (device_now === 'device_android') {
        $imEditPlus.fadeOut(function () {
            $imEditPlus.css({'transform': 'translate3d(0,0,0)', 'transition': 'none'})  
        })
    } else {
        $imEditPlus.removeClass('plus').hide()
    }
    $imEditBase.fadeIn(100)

    $imEditIcons.removeClass('active')
    $imEditTools.hide()
}

$("body").on("click", ".im_edit_icon", function (e) {
    var $el = $(e.currentTarget)
    var tool = $el.data('tool')
    $imEditIcons.removeClass('active')
    $el.addClass('active')
    $imEditTools.hide()
    $('.im_edit_tool[data-tool=' + tool + ']').show()
})



$(document).on('click', '.tool_face_pack', function (e) {
    var $el = $(e.currentTarget)
    console.log($el)
    var index = $el.index()
    var leftValue = $el.width() * index
    var face_pack = $el.data('face-pack')
    $('.tool_face_packs_border').css({'left': leftValue})
    $('.tool_face').hide()
    $('.tool_face[data-face-pack="' + face_pack + '"]').fadeIn()
})

$(document).on('click', '.tool_face_item', function (e) {
    var $el = $(e.currentTarget)
    $el.addClass('animate')
    var face_id = $el.data('face-id')
    var face_url = "http://img.plancats.com/" + face_id + '.png'
    var content = 'HWEBFACEIMG//' + encodeURIComponent(face_url)
    submitComment(content)
})

$(document).on('animationend', '.tool_face_item', function (e) {
    var $el = $(e.currentTarget)
    $el.removeClass('animate')
})
$(document).on('webkitAnimationEnd', '.tool_face_item', function (e) {
    var $el = $(e.currentTarget)
    $el.removeClass('animate')
})




function showMessageTip (msg) {
    console.log(msg)
}


// 选择聊天界面

// 创建聊天界面
function createConversation (roomId, imgUrl, roomName) {
    if(latestRoomIds.indexOf(roomId)==-1){
        var time_now = ((new Date()).getTime()/1000)
        rooms_info[roomId] = {
            "imgUrl": imgUrl,
            "roomName": roomName,
            "createtime": time_now,
            "finishtime": 0,
            "room_time_flag": time_now,
            "createuser":"",
            "finishuser":"",
            "createcommentsequence":"",
            "finishcommentsequence":"",
        }

        latestRoomIds.unshift(roomId)
        head = $('<div class="im_chat_head" data-room-id="' + roomId + '"></div>')[0]
        head.style.backgroundImage = 'url(' + imgUrl + ')'
        head.style.zIndex = baseZIndex + 1
        console.log(x, y)
        if (isExpanded) {
            var newX = expandedX(-1)
            var newY = expandedY(-1)
        } else {
            var newX = collapsedX(-1)
            var newY = collapsedY(-1)
        }
        setTransform(head, newX, newY)
        $chatHeads.prepend(head)

        var len = latestRoomIds.length
        if (len > MAXROOMS) {
            if (targetRoomId === latestRoomIds[len - 1]) {
                var delRoomId = latestRoomIds.splice(len - 2, 1)
            } else {
                var delRoomId = latestRoomIds.pop()
            }
            $chatHeads.children('[data-room-id=' + delRoomId + ']').remove()
            $rooms.children('.room[data-room-id='+ delRoomId +']').remove()
            last_comment_ids[delRoomId] = null
            rooms_info[delRoomId] = {}
            len = MAXROOMS
        }

        for (var i = 0; i < len; i++) {
            var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
            el.classList.add('transition')
            if (isExpanded) {
                var newX = expandedX(-1)
                var newY = expandedY(-1)
            } else {
                var newX = collapsedX(-1)
                var newY = collapsedY(-1)
            }
            setTransform(el, newX, newY)
            el.style.zIndex = baseZIndex - i
        }

        $rooms.append(
            '<div class="room" style="display:block;" data-room-id="' + roomId + '" data-room-name="' + roomName + '">' +
                '<div class="room_load_btn">往昔消息</div>' +
                '<div class="room_card"></div>' +
            '</div>')

        // $rooms.children().hide()
        // $rooms.children('[data-room-id=' + targetRoomId + ']').show()
        // $rooms.children('[data-room-id=' + targetRoomId + ']').on('scroll', onRoomScroll)

        // $chatName.text(rooms_info[targetRoomId].roomName)
        var index = latestRoomIds.indexOf(targetRoomId)
        var offset = r * (2 * index + 1) + 5 * (index + 1)
        $roomArrow.css({ 'right': offset + 'px' })

        $.ajax({
            url: 'http://www.hotpoor.org/api/comment/load',
            type: 'POST',
            dataType: 'json',
            data: {
                app: WX_APP,
                aim_id: roomId,
                comment_id: last_comment_ids[roomId],
            },
            success: function (data) {
                console.log(data)
                if(data.info == "ok"){
                    last_comment_ids[roomId] = data.last_comment_id
                    members_json_now = members_json
                    members_json_new = data.members
                    members_json = $.extend({}, members_json_now,members_json_new)
                    var comment, comments, _i, _len, _msg
                    comments = data.comments
                    for (_i = 0, _len = comments.length; _i < _len; _i++) {
                        comment = comments[(_len - _i - 1)]
                        _msg = [comment[3],{
                            "content": comment[4],
                            "nickname": members_json[comment[1]].nickname,
                            "headimgurl": members_json[comment[1]].headimgurl,
                            "time": comment[2],
                            "user_id": comment[1],
                            "tel": members_json[comment[1]].tel,
                            "plus": comment[5],
                            "sequence": comment[0],
                            "comment_id": data.comment_id,
                        },roomId]
                        loadMessage(_msg)
                        if (_i == 0) {
                            rooms_info[roomId]["latestComment"] = _msg
                            msg = _msg
                            item_text = ""
                            if(msg[0]=="AUDIO"){
                                item_text = "[语音]"+msg[1].plus.content
                            }else if(msg[0]=="COMMENT"){
                                if(msg[1].content.indexOf("HQWEBIMG//")>=0||msg[1].content.indexOf("HWEBIMG//")>=0){
                                    item_text = "[图片]"
                                }else if(msg[1].content.indexOf("HQWEBPANORAMA//")>=0||msg[1].content.indexOf("HWEBPANORAMA//")>=0){
                                    item_text = "[全景图片]"
                                }else{
                                    item_text = msg[1].content
                                }
                            }
                            item_text = msg[1].nickname +": "+item_text
                            $(".im_room_item[data-room-id='"+roomId+"']>.im_room_item_latest_msg").text(item_text)
                            $(".im_room_item[data-room-id='"+roomId+"']>.im_room_item_latest_time").text(formatDate(msg[1].time))
                        }
                    }
                    $(".room[data-room-id='"+roomId+"']>.room_card>.msg_time[data-time='"+rooms_info[roomId].createtime+"']").removeClass("msg_time_hide")
                    $(".msg[data-comment-flag='"+rooms_info[roomId].createcommentsequence+"']>.msg_headimg").removeClass("msg_headimg_hide")
                    $(".msg[data-comment-flag='"+rooms_info[roomId].createcommentsequence+"']>.msg_nickname_hide").removeClass("msg_nickname_hide")
                }
            },
            error: function (error) {
                console.log(error)
            }
        })
    }
    resetStyle()
    if (roomId !== initialRoomId) {
        joinMoreRooms([roomId])
    }
}




// 处理收起状态时，收到新消息时，切换头像，显示消息框，显示红标
function onMessageCollapsed (roomId, msg) {
    var content = msg[1].content
    content = content.replace(/\s/, '')
    content = escapeHTML(content)

    var index = latestRoomIds.indexOf(roomId)
    if (index !== -1) {
        targetRoomId = roomId
        latestRoomIds.splice(index, 1)
        latestRoomIds.unshift(roomId)

        if (!latestUnreadNumber[roomId]) { latestUnreadNumber[roomId] = 0 }
        latestUnreadNumber[roomId] += 1
        // notifyWhenCollapsed(roomId, content, latestUnreadNumber[roomId], 'number')
        notifyWhenCollapsed(roomId, content, latestUnreadNumber[roomId], 'text')

        var len = latestRoomIds.length
        for (var i = 0; i < len; i++) {
            var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
            el.classList.add('transition')
            setTransform(el, collapsedX(i), collapsedY(i))
            el.style.zIndex = baseZIndex - i
        }
    } else {
        targetRoomId = roomId
        latestRoomIds.unshift(roomId)
        var len = latestRoomIds.length

        latestUnreadNumber[roomId] = 1
        // notifyWhenCollapsed(roomId, content, 1, 'number')
        notifyWhenCollapsed(roomId, content, 1, 'text')

        var head = $('<div class="im_chat_head" data-room-id="' + roomId + '" style="background-image:url()"></div>')[0]
        setTransform(head, collapsedX(-1), collapsedY(-1))
        head.style.zIndex = baseZIndex + 1
        $chatHeads.prepend(head)

        if (len > MAXROOMS) {
            var delRoomId = latestRoomIds[len - 1]
            latestRoomIds.pop()
            $chatHeads.children('[data-room-id=' + delRoomId + ']').remove()
            $rooms.children('.room[data-room-id='+ delRoomId +']').remove()
            last_comment_ids[delRoomId] = null
            rooms_info[delRoomId] = {}
            len = MAXROOMS

            delete latestUnreadNumber[delRoomId]
            // notifyWhenCollapsed(delRoomId, content, 0, 'number')
            notifyWhenCollapsed(delRoomId, content, 0, 'text')
        }

        for (var i = 0; i < len; i++) {
            var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
            el.classList.add('transition')
            setTransform(el, collapsedX(i), collapsedY(i))
            el.style.zIndex = baseZIndex - i
        }
    }
}

// 处理展开状态时，收到新消息时，切换头像，显示红标
function onMessageExpanded (roomId, msg) {
    var content = msg[1].content
    content = content.replace(/\s/, '')
    content = escapeHTML(content)
    
    var index = latestRoomIds.indexOf(roomId)
    if (roomId === targetRoomId) {
        //
    } else if (index !== -1) {
        latestRoomIds.splice(index, 1)
        latestRoomIds.unshift(roomId)

        if (!latestUnreadNumber[roomId]) { latestUnreadNumber[roomId] = 0 }
        latestUnreadNumber[roomId] += 1
        notifyWhenExpanded(roomId, content, latestUnreadNumber[roomId], 'number')
        //notifyWhenExpanded(roomId, content, latestUnreadNumber[roomId], 'dot')

        var len = latestRoomIds.length
        for (var i = 0; i < len; i++) {
            var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
            el.classList.add('transition')
            setTransform(el, expandedX(i), expandedY(i))
            el.style.zIndex = baseZIndex - i
        }
    } else {
        latestRoomIds.unshift(roomId)
        var len = latestRoomIds.length

        latestUnreadNumber[roomId] = 1
        notifyWhenExpanded(roomId, content, 1, 'number')
        //notifyWhenExpanded(roomId, content, 1, 'dot')

        var head = $('<div class="im_chat_head" data-room-id="' + roomId + '" style="background-image:url()"></div>')[0]
        setTransform(head, expandedX(-1), expandedY(-1))
        head.style.zIndex = baseZIndex + 1
        $chatHeads.prepend(head)

        if (len > MAXROOMS) {
            if (targetRoomId === latestRoomIds[len - 1]) {
                var delRoomId = latestRoomIds.splice(len - 2, 1)
            } else {
                var delRoomId = latestRoomIds.pop()
            }

            delete latestUnreadNumber[delRoomId]
            notifyWhenExpanded(delRoomId, content, 0, 'number')
            //notifyWhenExpanded(delRoomId, content, 0, 'dot')

            $chatHeads.children('[data-room-id=' + delRoomId + ']').remove()
            $rooms.children('.room[data-room-id='+ delRoomId +']').remove()
            last_comment_ids[delRoomId] = null
            rooms_info[delRoomId] = {}
            len = MAXROOMS
        }

        for (var i = 0; i < len; i++) {
            var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
            el.classList.add('transition')
            setTransform(el, expandedX(i), expandedY(i))
            el.style.zIndex = baseZIndex - i
        }
    }
    var index = latestRoomIds.indexOf(targetRoomId)
    var offset = r * (2 * index + 1) + 5 * (index + 1)
    $roomArrow.css({ 'right': offset + 'px' })
}

var isExpanding = false
// 处理收起状态时，点击头像，隐藏消息框，展开头像，清除红标
function onClickCollapsed () {
    $(".msg_notice").remove()
    if (!isExpanding) {
        $("body").append("<div class='xialiwei_help_cover'></div>");
        isExpanding = true
        var _roomId = targetRoomId
        $cover.show()
        setTimeout(function () {
            $imBox.addClass('expand')
            $imBox.show()
            $roomArrow.css({ 'right': '35px' })
            $roomArrow.addClass('show')

            // $('.room[data-room-id=' + _roomId + ']>.room_card')[0].scrollIntoView(false)
            console.log(_roomId)
            var $currentRoom = $('.room[data-room-id=' + _roomId + ']')
            var $currentRoomCard = $currentRoom.find('.room_card')
            console.log($currentRoomCard)
            $currentRoomCard[0].scrollIntoView(false)
            if ($currentRoomCard.height() <= $currentRoom.height() - 20) {
                if (last_comment_ids[_roomId]) {
                    $currentRoom.find('.room_load_btn').show()
                }
            }
            setTimeout(function () {
                isExpanding = false
                isExpanded = true
                $logo.css({'visibility': 'visible'})
                $(".xialiwei_help_cover").remove();
            }, 300)
        }, timeout_rooms_show_plus)

        $body.addClass('expand')
        $logo.addClass('expand')
        $chatHeads.addClass('expand')

        var len = latestRoomIds.length
        for (var i = 0; i < len; i++) {
            var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
            el.classList.add('transition')
            setTransform(el, expandedX(i), expandedY(i))
            el.style.zIndex = baseZIndex - i
        }
        setTransform($logo[0], expandedX(len), expandedY(len))

        $logo.css({'visibility': 'visible'})

        latestUnreadNumber[_roomId] = 0
        notifyWhenExpanded(_roomId, '', 0, 'number')
        //notifyWhenExpanded(_roomId, '', 0, 'dot')

        $rooms.children().hide()
        var $room = $rooms.children('[data-room-id=' + _roomId + ']')
        $room.show()
        var roomName = $room.data('room-name')
        $chatName.text(roomName)

        isExpanded = true

    }
}

var isCollapsing = false
// 处理展开状态时，点击头像，收起头像
function onClickExpanded () {
    if (!isCollapsing) {
        $("body").append("<div class='xialiwei_help_cover'></div>");
        isCollapsing = true
        $cover.hide()
        $imBox.hide()
        $body.removeClass('expand')
        $logo.removeClass('expand')
        $chatHeads.removeClass('expand')

        var index = latestRoomIds.indexOf(targetRoomId)
        latestRoomIds.splice(index, 1)
        latestRoomIds.unshift(targetRoomId)

        var len = latestRoomIds.length
        for (var i = 0; i < len; i++) {
            var roomId = latestRoomIds[i]
            var el = $chatHeads.children('[data-room-id=' + roomId + ']')[0]
            el.classList.add('transition')
            setTransform(el, collapsedX(i), collapsedY(i))
            el.style.zIndex = baseZIndex - i
        }
        setTransform($logo[0], collapsedX(len), collapsedY(len))
        // arrow
        $roomArrow.removeClass('show')
        setTimeout(function () {
            isCollapsing = false
            isExpanded = false
            $logo.css({'visibility': 'hidden'})
            $(".xialiwei_help_cover").remove();
        }, 300)
        isExpanded = false
    }
}

var isSwitching = false
// 处理展开状态时，点击头像，切换 room
function onClickSwitch (roomId) {
    if (!isSwitching) {
        isSwitching = true
        console.log('click switch')
        targetRoomId = roomId
        $rooms.children().hide()
        var $room = $rooms.children('[data-room-id=' + roomId + ']')
        $room.show()
        var roomName = $room.data('room-name')
        $chatName.text(roomName)

        var index = latestRoomIds.indexOf(targetRoomId)
        var offset = r * (2 * index + 1) + 5 * (index + 1)
        $roomArrow.css({ 'right': offset + 'px' })

        latestUnreadNumber[roomId] = 0
        // notifyWhenExpanded(roomId, '', 0, 'number')
        notifyWhenExpanded(roomId, '', 0, 'dot')
        isSwitching = false
    }
}



function resetStyle () {
    var len = latestRoomIds.length
    for (var i = 0; i < len; i++) {
        var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
        el.style.zIndex = baseZIndex - i
        var _x, _y
        if (isExpanded) {
            _x = expandedX(i)
            _y = expandedY(i)
        } else {
            _x = collapsedX(i)
            _y = collapsedY(i)
        }
        setTransform(el, _x, _y)
    }
    if (isExpanded) {
        _x = expandedX(len)
        _y = expandedY(len)
    } else {
        _x = collapsedX(len)
        _y = collapsedY(len)
    }
    setTransform($logo[0], _x, _y)
}

function setTransform(el, x, y) {
   var value = 'translate3d(' + x + 'px,' + y + 'px,0)'
   el.style.transform = value
   el.style.webkitTransform = value
}
function collapsedX (i) {
    return maxX + 4 * i
}
function collapsedY (i) {
    return y
}
function expandedX (i) {
    return w - r * (2 * i + 1) - 5 * (i + 1)
}
function expandedY (i) {
    return r + 10
}

function move (_x, _y) {
    // console.log('move')
    var els = []
    var len = latestRoomIds.length
    for (var i = 0; i < len; i++) {
        var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
        els.push(el)
    }
    _callLater(els, function (el, index) {
        setTransform(el, _x + 4 * index, _y)
    })
    x = _x;
    y = _y;
}

function moveTogether (_x, _y) {
    // console.log('move together')
    var len = latestRoomIds.length
    for (var i = 0; i < len; i++) {
        var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
        setTransform(el, _x + 4 * i, _y)
    }
    x = _x;
    y = _y;
}


function onMoveEnd () {
    var els = []
    for (var i = 0; i < latestRoomIds.length; i++) {
        var el = $chatHeads.children('[data-room-id=' + latestRoomIds[i] + ']')[0]
        els.push(el)
    }
    _callLater(els, function (el, index) {
        el.classList.add('transition')
        setTransform(el, collapsedX(index), collapsedY(index))
    })
    setTransform($logo[0], collapsedX(els.length), collapsedY(els.length))
    x = maxX
}


function _requestFrame(n, callback) {
    if (n > 0) {
        requestAnimationFrame(function() {
            _requestFrame(n-1, callback)
        })
    } else if (n === 0) {
        callback()
    }
}

function _callLater(els, callback) {
    var len = els.length
    if (len) {
        function _call(i) {
            callback(els[i], i)
            i += 1
            if (i < len) {
                _requestFrame(3, function () {
                    _call(i)
                })
            }
        }
        _call(0)
    }
}


/**
 * 加载历史消息
 */
var isLoadingMore = false
var $roomLoadTip = $('.im_room_load_tip')

function loadHistory (currentRoomId) {
    isLoadingMore = true
    $roomLoadTip.text("消息加载中...").fadeIn()
    $.ajax({
        url: 'http://www.hotpoor.org/api/comment/load',
        type: 'POST',
        dataType: 'json',
        data: {
            app: WX_APP,
            aim_id: currentRoomId,
            comment_id: last_comment_ids[currentRoomId],
        },
        success: function (data) {
            console.log(data)
            if(data.info == "ok"){
                last_comment_ids[currentRoomId] = data.last_comment_id
                members_json_now = members_json
                members_json_new = data.members
                members_json = $.extend({}, members_json_now,members_json_new)
                var comment, comments, _i, _len, _msg
                comments = data.comments
                for (_i = 0, _len = comments.length; _i < _len; _i++) {
                    comment = comments[(_len - _i - 1)]
                    _msg = [comment[3],{
                        "content": comment[4],
                        "nickname": members_json[comment[1]].nickname,
                        "headimgurl": members_json[comment[1]].headimgurl,
                        "time": comment[2],
                        "user_id": comment[1],
                        "tel": members_json[comment[1]].tel,
                        "plus": comment[5],
                        "sequence": comment[0],
                        "comment_id": data.comment_id,
                    },currentRoomId]
                    loadMessage(_msg)
                }
                $(".room[data-room-id='"+currentRoomId+"']>.room_card>.msg_time[data-time='"+rooms_info[currentRoomId].createtime+"']").removeClass("msg_time_hide")
                $(".msg[data-comment-flag='"+rooms_info[currentRoomId].createcommentsequence+"']>.msg_headimg").removeClass("msg_headimg_hide")
                $(".msg[data-comment-flag='"+rooms_info[currentRoomId].createcommentsequence+"']>.msg_nickname_hide").removeClass("msg_nickname_hide")
                var scroll_flag = data.comment_id + '_' + comments[comments.length-1][0]
                var $el = $("[data-comment-flag='"+ scroll_flag +"']")
                console.log(currentRoomId)
                console.log($el[0].offsetTop)
                if ($el.length == 1) {
                    $('.room[data-room-id="' + currentRoomId + '"]').animate({
                        "scrollTop": $el[0].offsetTop })
                }
                isLoadingMore = false
                $roomLoadTip.text('加载成功！')
                setTimeout(function () {
                    $roomLoadTip.fadeOut()
                }, 1000)
            } else {
                isLoadingMore = false
                $roomLoadTip.text('加载失败！')
                setTimeout(function () {
                    $roomLoadTip.fadeOut()
                }, 1000)             
            }
        },
        error: function (error) {
            console.log(error)
            isLoadingMore = false
            $roomLoadTip.text('加载失败！')
            setTimeout(function () {
                $roomLoadTip.fadeOut()
            }, 1000)
        }
    })
}

function onRoomScroll (e) {
    var $el = $(e.currentTarget)
    var scrollTop = $el.scrollTop()
    if (scrollTop < 10 && !isLoadingMore) {
        if (last_comment_ids[targetRoomId]) {
            var $el = $('.room[data-room-id="' + targetRoomId + '"]')
            $el.find('.room_load_btn').hide()
            loadHistory(targetRoomId)
        } else {

        }
    }
}

$("body").on("click", ".room_load_btn", function (e) {
    var $el = $(e.currentTarget)
    $el.fadeOut()
    var roomId = $el.parent().data('room-id')
    loadHistory(roomId)
})

/**
 * æ¶æ¯éç¥
 */

// var noDisturbList = []  // ä¸ææ° roomId åè¡¨
var latestUnreadNumber = {}
var notifyWhenCollapsed_timeout = null
var notifyWhenCollapsed_timeout_fadeOut = null
function notifyWhenCollapsed (roomId, content, number, type) {
    console.log(number)
    var $el = $chatHeads.children('[data-room-id=' + roomId + ']')
    if (type === 'text') {
        var $head = $chatHeads.children('[data-room-id=' + roomId + ']')
        if (number == 0) {
            $head.children().remove()
        } else {
            $head.children(".msg_number").remove()
            var $msgNumber = $('<span class="msg_number">' + number + '</span>')
            $head.append($msgNumber)
        }

        $(".msg_notice").remove()
        var msgTip = $('<div class="msg_notice" style="top:'+(y - 15) + 'px">' +
            '<span class="msg_notice_text">' + content + '</span>' +
            '</div>')[0]
        $("body").append(msgTip)
        clearTimeout(notifyWhenCollapsed_timeout)
        clearTimeout(notifyWhenCollapsed_timeout_fadeOut)
        notifyWhenCollapsed_timeout = setTimeout(function () {
            $(msgTip).css({'transform': 'scale(1.0)','-webkit-transform': 'scale(1.0)'})
            notifyWhenCollapsed_timeout_fadeOut = setTimeout(function(){
                $(msgTip).fadeOut(500,function(){this.remove()})
            },2000)
        }, 200)
    } else if (type === 'number') {
        var $head = $chatHeads.children('[data-room-id=' + roomId + ']')
        if (number == 0) {
            $head.children().remove()
        } else {
            var $msgNumber = $('<span class="msg_number">' + number + '</span>')
            $head.append($msgNumber)
        }
    } else if (type === 'dot') {
        var $head = $chatHeads.children('[data-room-id=' + roomId + ']')
        if (number == 0) {
            $head.children().remove()
        } else {
            var $msgNumber = $('<span class="msg_dot"></span>')
            $head.append($msgNumber)
        }
    }
}

function notifyWhenExpanded (roomId, content, number, type) {
    if (type === 'dot') {
        var $head = $chatHeads.children('[data-room-id=' + roomId + ']')
        if (number == 0) {
            $head.children().remove()
        } else {
            var $msgNumber = $('<span class="msg_dot"></span>')
            $head.append($msgNumber)
        }
    } else if (type === 'number') {
        var $head = $chatHeads.children('[data-room-id=' + roomId + ']')
        if (number == 0) {
            $head.children().remove()
        } else {
            var $msgNumber = $('<span class="msg_number">' + number + '</span>')
            $head.append($msgNumber)
        }
    }
}



/**
 * 全景
 */

showPanorama_timeout = null
function showPanorama(url, width, height) {
    $('.pop_card_cover').show()
    $('.pop_card_cancel').show()
    clearTimeout(showPanorama_timeout)
    $('.pop_card').empty().append(
        '<div class="panorama_view_container clearfix" style="width: ' + width + 'px;height: ' + height + 'px;max-height:'+parseInt((h*0.9)- 90)+'px;">' +
            '<iframe class="panorama_view" style="width:100%;height:100%;border: none;" src=""></iframe>' +
        '</div>')
    showPanorama_timeout = setTimeout(function(){
        $(".panorama_view").attr("src",url)
    },300)

}
function showImage(url, width, height) {

    $('.pop_card_cover').show()
    $('.pop_card_cancel').show()
    $('.pop_card').empty().append(
        '<div class="image_view_container clearfix" style="max-width: ' + width + 'px;height:auto;">' +
            '<img crossorigin="Anonymous" class="image_view" style="width:100%;height:auto;border: none;" src="' + url + '">' +
        '</div>')
}
window.onload = function(){
    $("body").on("click", ".msg_panorama_thumbnail_wrap", function (e) {
        var url = e.currentTarget.dataset.url
        var width = parseInt(w * 0.9)
        var height = parseInt(width * 2 / 3) - 90/2
        var url_base = "http://hotpoor.org/home/tools/webgl_panorama_dualfisheye?url="+url+"&title=HOTPOOR&target_z=100&move=true&target_y=100&target_x=45"
        // "&screen_width="+width+"&screen_height="+height
        // var url_base2 = "http://www.hotpoor.org/home/tools/panorama?url="+url
        showPanorama(url_base, width, height)
    })
    $("body").on("click", ".msg_image_thumbnail_wrap", function (e) {
        var url = e.currentTarget.dataset.url
        var width = parseInt(w * 0.9)
        var height = parseInt(width * 2 / 3)
        var url_base = url
        showImage(url_base, width, height)
    })
    $("body").on("click", ".pop_card_wrap,.pop_card_cancel", function (e) {
        // if (e.target === e.currentTarget) {
            $('.pop_card_cover').hide()
            $('.pop_card_cancel').hide()
            clearTimeout(showPanorama_timeout)
            var frame = $(".panorama_view")
            var el = frame[0]
            if (el) {
                el.contentWindow.document.write('');
                el.contentWindow.document.clear();
                var p = el.parentNode;
                p.removeChild(el);
            }
            // cycleClear()
        // }
    })
    // cycleClear_timeout = null
    // function cycleClear() {
    //     try {
    //         var frame = $(".panorama_view")
    //         var el = frame[0]
    //         if (el) {
    //             el.contentWindow.document.write('');
    //             el.contentWindow.document.clear();
    //             var p = el.parentNode;
    //             p.removeChild(el);
    //         }
    //     } catch (e) {
    //         console.log("再来一次")
    //         cycleClear_timeout = setTimeout(cycleClear, 100);
    //     }
    // }
}

/**
 * 图片上传
 */
// var uploadFileType
var uploadFileType = null
var uploadFileRoomId = null
$("body").on("click", ".tool_image_btn", function (e) {
    var el = e.target
    uploadFileType = el.dataset.name
    console.log("======\n")
    console.log(uploadFileType)
    console.log("======\n")
    uploadFileRoomId = targetRoomId
    $("#img_add_upload").click()

})
$("body").on("mousemove", ".tool_image_btn", function (e) {
    $(".tool_image_btn").removeClass("tool_image_btn_here")
    $(this).addClass("tool_image_btn_here")
})
$("body").on("mouseout", ".tool_image_btn", function (e) {
    $(".tool_image_btn").removeClass("tool_image_btn_here")
})
$("body").on("touchmove", ".tool_image_btn", function (e) {
    $(".tool_image_btn").removeClass("tool_image_btn_here")
    $(this).addClass("tool_image_btn_here")
})
$(window).on("touchend", function (e) {
    $(".tool_image_btn").removeClass("tool_image_btn_here")
})




var root;

root = typeof exports !== "undefined" && exports !== null ? exports : this;

$(function() {
  var CDN_PREFIX, file_lists, handleFileSelect;
  CDN_PREFIX = "http://image.hotpoor.org";
  file_lists = [];
  if (window.File && window.FileList && window.FileReader && window.Blob && window.Worker) {
    handleFileSelect = function(evt) {
      var content_type, endingByte, file_index, files, img_add, startingByte, uploadFile, _ref, _room_id;
      img_add = (_ref = $(this).attr("id") === "img_add_upload") != null ? _ref : {
        "true": false
      };
      _room_id = uploadFileRoomId;
      content_type = "HQWEBIMG//";
      if (uploadFileType === "panorama") {
        content_type = "HQWEBPANORAMA//";
      }
      evt.stopPropagation();
      evt.preventDefault();
      files = null;
      if (evt.target.files) {
        files = evt.target.files;
      } else {
        files = evt.dataTransfer.files;
      }
      file_lists.push(files);
      file_index = 0;
      startingByte = 0;
      endingByte = 0;
      uploadFile = function(file) {
        var reader, tempfile, type, updateProgress, uploadNextFile, xhrProvider;
        type = file.type ? file.type : 'n/a';
        reader = new FileReader();
        tempfile = null;
        startingByte = 0;
        console.log("正在上传第一张图片");
        xhrProvider = function() {
          var xhr;
          xhr = jQuery.ajaxSettings.xhr();
          if (xhr.upload) {
            xhr.upload.addEventListener('progress', updateProgress, false);
          }
          return xhr;
        };
        updateProgress = function(evt) {
          return console.log("Uploading file " + (file_index + 1) + " of " + files.length + " at " + ((startingByte + (endingByte - startingByte) * evt.loaded / evt.total) / file.size * 100) + "%");
        };
        uploadNextFile = function() {
          var obj;
          console.log("正在要上传下一张图片");
          file_index += 1;
          if (file_index < files.length) {
            uploadFile(files[file_index]);
            console.log("===");
            console.log(file_index);
            return console.log("===|||");
          } else {
            file_lists.shift();
            if (file_lists.length > 1) {
              file_index = 0;
              files = file_lists[0];
              uploadFile(files[file_index]);
              console.log("===+++");
              console.log(file_index);
              return console.log("===|||");
            } else {
              console.log("===>>>");
              obj = document.getElementById('img_add_upload');
              return obj.outerHTML = obj.outerHTML;
            }
          }
        };
        reader.onload = function(evt) {
          var bin, content, worker;
          content = evt.target.result.slice(evt.target.result.indexOf("base64,") + 7);
          bin = atob(content);
          worker = new Worker("/static/js/md5.js");
          worker.onmessage = function(event) {
            var Qiniu_UploadUrl, Qiniu_UploadUrl_https, md5, worker_aim_url;
            md5 = event.data;
            Qiniu_UploadUrl_https = "https://up.qbox.me";
            if (window.location.protocol === "https:") {
              Qiniu_UploadUrl = Qiniu_UploadUrl_https;
            } else {
              Qiniu_UploadUrl = "http://up.qiniu.com";
            }
            worker_aim_url = "/api/image/check";
            return $.post(worker_aim_url, {
              "aim_id": _room_id,
              "md5": md5
            }, function(data) {
              var Qiniu_upload, upload_token;
              if (files.length === 1) {
                console.log("正在上传1张图片");
                if (data["exists"]) {
                  content = content_type + md5;
                  return $.ajax({
                    url: '/api/comment/submit',
                    type: 'POST',
                    dataType: 'json',
                    data: {
                      "app": WX_APP,
                      "aim_id": _room_id,
                      "content": content
                    },
                    success: function(data) {
                      var obj;
                      console.log(data);
                      obj = document.getElementById('img_add_upload');
                      return obj.outerHTML = obj.outerHTML;
                    },
                    error: function(error) {
                      return console.log(error);
                    }
                  });
                }
              } else {
                if (file_index + 1 === files.length) {
                  console.log("正在上传最后一张图片");
                  if (data["exists"]) {
                    content = content_type + md5;
                    return $.ajax({
                      url: '/api/comment/submit',
                      type: 'POST',
                      dataType: 'json',
                      data: {
                        "app": WX_APP,
                        "aim_id": _room_id,
                        "content": content
                      },
                      success: function(data) {
                        var obj;
                        console.log(data);
                        obj = document.getElementById('img_add_upload');
                        return obj.outerHTML = obj.outerHTML;
                      },
                      error: function(error) {
                        return console.log(error);
                      }
                    });
                  }
                } else {
                  console.log("正在上传" + (file_index + 1) + "/" + files.length + "张图片");
                  if (data["exists"]) {
                    content = content_type + md5;
                    return $.ajax({
                      url: '/api/comment/submit',
                      type: 'POST',
                      dataType: 'json',
                      data: {
                        "app": WX_APP,
                        "aim_id": _room_id,
                        "content": content
                      },
                      success: function(data) {
                        return console.log(data);
                      },
                      error: function(error) {
                        return console.log(error);
                      },
                      complete: function() {
                        return uploadNextFile();
                      }
                    });
                  }
                }
              }
              upload_token = data["token"];
              Qiniu_upload = function(f, token, key) {
                var formData, startDate, xhr;
                xhr = new XMLHttpRequest();
                xhr.open('POST', Qiniu_UploadUrl, true);
                formData = new FormData();
                if (key !== null && key !== void 0) {
                  formData.append('key', key);
                }
                formData.append('token', token);
                formData.append('file', f);
                xhr.upload.addEventListener("progress", function(evt) {
                  var formatSpeed, nowDate, percentComplete, taking, uploadSpeed, x, y;
                  if (evt.lengthComputable) {
                    nowDate = new Date().getTime();
                    taking = nowDate - startDate;
                    x = evt.loaded / 1024;
                    y = taking / 1000;
                    uploadSpeed = x / y;
                    if (uploadSpeed > 1024) {
                      formatSpeed = (uploadSpeed / 1024).toFixed(2) + "Mb\/s";
                    } else {
                      formatSpeed = uploadSpeed.toFixed(2) + "Kb\/s";
                    }
                    percentComplete = Math.round(evt.loaded * 100 / evt.total);
                    return console.log(percentComplete, ",", formatSpeed);
                  }
                }, false);
                xhr.onreadystatechange = function(response) {
                  var blkRet;
                  if (xhr.readyState === 4 && xhr.status === 200 && xhr.responseText !== "") {
                    blkRet = JSON.parse(xhr.responseText);
                    return $.post("/api/image/add", {
                      "aim_id": _room_id,
                      "md5": md5
                    }, function() {
                      content = "HQWEBIMG//" + md5;
                      $.ajax({
                        url: '/api/comment/submit',
                        type: 'POST',
                        dataType: 'json',
                        data: {
                          "app": WX_APP,
                          "aim_id": _room_id,
                          "content": content
                        },
                        success: function(data) {
                          return console.log(data);
                        },
                        error: function(error) {
                          return console.log(error);
                        }
                      });
                      return uploadNextFile();
                    });
                  }
                };
                startDate = new Date().getTime();
                return xhr.send(formData);
              };
              return Qiniu_upload(file, upload_token, _room_id + "_" + md5);
            });
          };
          return worker.postMessage(bin);
        };
        return reader.readAsDataURL(file);
      };
      if (file_lists.length >= 1) {
        return uploadFile(files[file_index]);
      }
    };
    return $("body").on("change", "#img_add_upload", handleFileSelect);
  }
});









/**
 * 微信录音
 */



var weixinRecordingMode = 'single'
var startWeixinRecording_time=0

var $recordRing = $('.wx_record_ring')
var $recordStart = $('#wx_record_start')
var $recordStop = $('#wx_record_stop')
var $recordUpload = $('#wx_record_upload')
var $recordCancel = $('#wx_record_cancel')
var $recordInfo = $('.wx_record_info')
var $modeSingle = $("#wx_record_mode_single")
var $modeContinue = $("#wx_record_mode_continue")

$("body").on("click", "#wx_record_mode_single", function (e) {
    weixinRecordingMode = 'single'
    $modeSingle.addClass("active")
    $modeContinue.removeClass("active")
})
$("body").on("click", "#wx_record_mode_continue", function (e) {
    weixinRecordingMode = 'continue'
    $modeSingle.removeClass("active")
    $modeContinue.addClass("active")
})


$("body").on("animationend", ".wx_record_ring", function (e) {
    $(e.currentTarget).removeClass('animate')
})
$("body").on("webkitAnimationEnd", ".wx_record_ring", function (e) {
    $(e.currentTarget).removeClass('animate')
})

var recordStatus = 'notRecord' // 'recording', 'uploading'
var recordActionStop = false
$("body").on("click", "#wx_record_start", function (e) {
    var _room_id=targetRoomId
    if (recordStatus === 'notRecord') {
        recordActionStop = false
        wx.startRecord({
            cancel: function() {},
            success: function() {
                recordStatus = 'recording'
                startCount()
                $recordStart.hide()
                $recordStop.show()
                $recordRing.addClass('animate')
                $recordCancel.addClass('active')
                $recordInfo.text('完成录音并发送')

                startWeixinRecording_time = new Date().getTime();
                wx.onVoiceRecordEnd({
                    complete: function(res) {
                        if (recordStatus === 'recording') {
                            recordStatus = 'uploading'
                            stopCount()
                            $recordStop.hide()
                            $recordUpload.show()
                            $recordCancel.removeClass('active')
                            $recordInfo.text('上传中')

                            var localId, localId_now;
                            localId = res.localId;
                            localId_now = localId;
                            return StopWeixinRecordingAuto(localId_now,_room_id);
                        }
                      }
                });
            }
        });
    }
})

$("body").on("click", "#wx_record_stop", function (e) {
    var _room_id = targetRoomId
    if (recordStatus === 'recording') {
        recordActionStop = true
        recordStatus = 'uploading'
        stopCount()
        $recordStop.hide()
        $recordUpload.show()
        $recordCancel.removeClass('active')
        $recordInfo.text('上传中')

        wx.stopRecord({
            success: function(res) {
                var localId, localId_now;
                localId = res.localId;
                localId_now = localId;
                return StopWeixinRecordingAuto(localId_now,_room_id);
            }
        })
    }
})

$("body").on("click", "#wx_record_cancel", function () {
    if (recordStatus === 'recording') {
        recordActionStop = true
        recordStatus = 'notRecord'
        stopCount()
        $recordStop.hide()
        $recordStart.show()
        $recordInfo.text("开始录音")
        $recordCancel.removeClass('active')
        wx.stopRecord({
            success: function (res) {
                
            }
        })
    } else if (recordStatus === 'notRecord') {
        hideEditPlus()
    }
})



var $recordTimer = $('.wx_record_timer')
var timer = null
function startCount () {
    var count = parseInt($recordTimer.text())
    if (!timer) {
        timer = setInterval(function () {
            // count -= 1
            var time = new Date().getTime()
            count = parseInt((60000 + startWeixinRecording_time - time) / 1000)
            $recordTimer.text(count + 's')
            if (count === 2) {
                $('#wx_stopRecord').click()
            }
        }, 1000)
    }
}
function stopCount () {
    clearInterval(timer)
    timer = null
    $recordTimer.text('60s')
}

function StopWeixinRecordingAuto(localId_now, _room_id) {
    stopWeixinRecording_time = new Date().getTime();
    data_info = {
        "name": new Date().getTime() + Math.random(),
        "duration": (stopWeixinRecording_time - startWeixinRecording_time) / 1000.0
    };
    wx.uploadVoice({
        localId: localId_now,
        isShowProgressTips: 1,
        success: function(res) {
            $.post("/api/audio/weixin_upload", {
                "aim_id": _room_id,
                "media_id": res.serverId,
                "name": data_info.name,
                "duration": data_info.duration,
                "app": WX_APP
            }, function(data) {
                if (recordStatus === 'uploading') {
                    wx_upload_wait_html = '<div class="msg_self audio_wait" data-media="'+res.serverId+'"><span>语音正在上传...</span></div>'
                    $(".room[data-room-id='"+_room_id+"']>.room_card").append(wx_upload_wait_html)
                    $('.room[data-room-id=' + _room_id + ']').stop().animate({"scrollTop":$(".msg_self.audio_wait[data-media='"+res.serverId+"']")[0].offsetTop})
                    recordStatus = 'notRecord'
                    $recordUpload.hide()
                    $recordStart.show()
                    $recordInfo.text('开始录音')
                    $('.wx_record_cancel_wrap').show()
                    if (weixinRecordingMode === 'continue') {
                        if (recordActionStop) {
                            recordActionStop = false
                        } else {
                            $recordStart.click()
                        }
                    }
                }
                //alert("微信上传成功");
            });
        }
    });
}


/**
 * 播放录音
 */
var globalVolume = 1

var $volumeControl = $('.tool_setting_volume_control')
var $volumeProgress = $('.tool_setting_volume_progress')
var $volumeDot = $('.tool_setting_volume_dot')
var $volumeIcon = $('.tool_setting_volume_icon')

var volumeDotStart = false
var volumeWidth
var volumeStartX
var volumeOffsetX

var isMuted = false
$volumeIcon.on("click", function (e) {
    if (!isMuted) {
        isMuted = true
        audioEl.muted = true
        $volumeIcon.addClass('mute')
    } else {
        isMuted = false
        audioEl.muted = false
        $volumeIcon.removeClass('mute')
    }
})


$volumeDot.on("mousedown", function (e) {
    volumeDotStart = true
    e.preventDefault()
    e.stopPropagation()
    volumeWidth = $volumeControl.width()
    volumeStartX = e.clientX
    volumeOffsetX = volumeWidth * globalVolume
})
$(window).on("mousemove", function (e) {
    if (volumeDotStart) {
        e.preventDefault()
        var value = volumeOffsetX + (e.clientX - volumeStartX)
        if (value <= 0) {
            value = 0
        }
        if (value >= volumeWidth) {
            value = volumeWidth
        }
        var vol = value / volumeWidth
        $volumeProgress.css({'width': 100 * vol + '%'})
        $volumeDot.css({'left': 100 * vol + '%'})
        globalVolume = vol
        audioEl.volume = vol
    }
})
$(window).on("mouseup", function (e) {
    if (volumeDotStart) {
        volumeDotStart = false
        if (globalVolume > 0) {
            isMuted = false
            audioEl.muted = false
            $volumeIcon.removeClass('mute')
        }
    }
})

$volumeDot.on("touchstart", function (e) {
    volumeDotStart = true
    e.preventDefault()
    e.stopPropagation()
    volumeWidth = $volumeControl.width()
    var touch = e.originalEvent.targetTouches[0]
    volumeStartX = touch.clientX
    volumeOffsetX = volumeWidth * globalVolume
})
$(window).on("touchmove", function (e) {
    console.log('aaa')
    if (volumeDotStart) {
        e.preventDefault()
        var touch = e.originalEvent.touches[0]
        var value = volumeOffsetX + (touch.clientX - volumeStartX)
        console.log(value)
        if (value <= 0) {
            value = 0
        }
        if (value >= volumeWidth) {
            value = volumeWidth
        }
        var vol = value / volumeWidth
        $volumeProgress.css({'width': 100 * vol + '%'})
        $volumeDot.css({'left': 100 * vol + '%'})
        globalVolume = vol
        audioEl.volume = vol
    }
})
$(window).on("touchend", function (e) {
    if (volumeDotStart) {
        volumeDotStart = false
        if (globalVolume > 0) {
            isMuted = false
            audioEl.muted = false
            $volumeIcon.removeClass('mute')
        }
    }
})


var audioEl = document.createElement('audio')
var $currEl = null 

audioEl.ontimeupdate = function () {
    setAudioStyle()
}
audioEl.onended = function () {
    resetAudioStyle()
    $currEl.removeClass('playing')
}

$(document).on('click', '.msg_content.audio', function (e) {
    $el = $(e.currentTarget)
    var src = $el.data('audio-src')
    var currSrc = audioEl.currentSrc

    if (src == currSrc) {
        if (audioEl.paused) {
            $el.addClass('playing')
            audioEl.play()
        } else {
            $el.removeClass('playing')
            audioEl.pause()
        }
    } else {
        if ($currEl) {
            $currEl.removeClass('playing')
            resetAudioStyle()
        }
        $currEl = $el
        $el.addClass('playing')
        audioEl.src = src
        audioEl.play()
    }
})


function setAudioStyle () {
    if (!isNaN(audioEl.duration)) {
        var t = parseInt(audioEl.duration - audioEl.currentTime)
        var w = parseInt(200 * audioEl.currentTime / audioEl.duration)
        $currEl.find('.msg_audio_time').text(t + '"')
        $currEl.find('.msg_audio_cover').css({'width': w + 'px'})
    }
}
function resetAudioStyle () {
    $currEl.find('.msg_audio_time').text(parseInt(audioEl.duration) + '"')
    $currEl.find('.msg_audio_cover').css({'width': '0'})
}



/**
 * Utils 工具函数
 */

function escapeHTML (str) {
    return $('<div></div>').text(str).html()
}

function formatDate (now) {
    var comment_time_now, date, hour, minute, month, now_date, year;
    now_date = new Date(now * 1000);
    comment_time_now = new Date;
    year = now_date.getFullYear();
    month = now_date.getMonth() + 1;
    date = now_date.getDate();
    hour = now_date.getHours();
    minute = now_date.getMinutes();
    if (hour < 10) {
        hour = "0" + hour
    }
    if (minute < 10) {
        minute = "0" + minute
    }
    if (comment_time_now.getFullYear() === year && comment_time_now.getMonth() + 1 === month && comment_time_now.getDate() === date) {
        return hour + ":" + minute
    }
    if (comment_time_now.getFullYear() === year) {
        return month + "月" + date + "日 " + hour + ":" + minute
    }
    return year + "年" + month + "月" + date + "日 " + hour + ":" + minute
}
function reload(){
    var msg = ['RELOAD', {"content":"HOME//CMD//RELOAD"}, initialRoomId]
    ws.send(JSON.stringify(msg))
}


// 电脑端从粘贴板中截图上传
$("body").on("paste", ".im_edit_content", function(evt) {
  var blob, reader, t, _room_id;
  if ((evt.originalEvent || evt).clipboardData.types[0] === "Files") {
    t = (evt.originalEvent || evt).clipboardData;
    if (t.items[0].kind === "file" && t.items[0].type.match(/^image\//i)) {
      blob = t.items[0].getAsFile();
      reader = new FileReader();
      _room_id = targetRoomId;
      reader.onload = function() {
        var uploadFile;
        uploadFile = function(file) {
          var endingByte, file_index, file_lists, files, startingByte, tempfile, type, updateProgress, uploadNextFile, xhrProvider;
          file_lists = [];
          files = file;
          file_lists.push(files);
          file_index = 0;
          startingByte = 0;
          endingByte = 0;
          type = file.type ? file.type : 'n/a';
          reader = new FileReader();
          tempfile = null;
          startingByte = 0;
          xhrProvider = function() {
            var xhr;
            xhr = jQuery.ajaxSettings.xhr();
            if (xhr.upload) {
              xhr.upload.addEventListener('progress', updateProgress, false);
            }
            return xhr;
          };
          updateProgress = function(evt) {
            return console.log("Uploading file " + (file_index + 1) + " of " + files.length + " at " + ((startingByte + (endingByte - startingByte) * evt.loaded / evt.total) / file.size * 100) + "%");
          };
          uploadNextFile = function() {
            file_index += 1;
            if (file_index < files.length) {
              return uploadFile(files[file_index]);
            } else {
              file_lists.shift();
              if (file_lists.length > 0) {
                file_index = 0;
                files = file_lists[0];
                return uploadFile(files[file_index]);
              }
            }
          };
          reader.onload = function(evt) {
            var bin, content, worker;
            content = evt.target.result.slice(evt.target.result.indexOf("base64,") + 7);
            bin = atob(content);
            worker = new Worker("/static/js/md5.js");
            worker.onmessage = function(event) {
              var Qiniu_UploadUrl, Qiniu_UploadUrl_https, md5;
              md5 = event.data;
              Qiniu_UploadUrl_https = "https://up.qbox.me";
              if (window.location.protocol === "https:") {
                Qiniu_UploadUrl = Qiniu_UploadUrl_https;
              } else {
                Qiniu_UploadUrl = "http://up.qiniu.com";
              }
              return $.post("/api/image/check", {
                "aim_id": _room_id,
                "md5": md5
              }, function(data) {
                var Qiniu_upload, upload_token;
                if (files.length === 1) {
                  if (data["exists"]) {
                    return;
                  }
                } else {
                  if (file_index + 1 === files.length) {
                    if (data["exists"]) {
                        console.log("file_index+1 == files.length");
                        return;
                    }
                  } else {
                    if (data["exists"]) {
                        console.log("md5 already")
                        content = "HQWEBIMG//" + md5;
                        return $.ajax({
                          url: '/api/comment/submit',
                          type: 'POST',
                          dataType: 'json',
                          data: {
                            "app": WX_APP,
                            "aim_id": _room_id,
                            "content": content
                          },
                          success: function(data) {
                            return console.log(data);
                          },
                          error: function(error) {
                            return console.log(error);
                          }
                        });
                        // return uploadNextFile();
                    }
                  }
                }
                upload_token = data["token"];
                Qiniu_upload = function(f, token, key) {
                  var formData, startDate, xhr;
                  xhr = new XMLHttpRequest();
                  xhr.open('POST', Qiniu_UploadUrl, true);
                  formData = new FormData();
                  if (key !== null && key !== void 0) {
                    formData.append('key', key);
                  }
                  formData.append('token', token);
                  formData.append('file', f);
                  xhr.upload.addEventListener("progress", function(evt) {
                    var formatSpeed, nowDate, percentComplete, taking, uploadSpeed, x, y;
                    if (evt.lengthComputable) {
                      nowDate = new Date().getTime();
                      taking = nowDate - startDate;
                      x = evt.loaded / 1024;
                      y = taking / 1000;
                      uploadSpeed = x / y;
                      if (uploadSpeed > 1024) {
                        formatSpeed = (uploadSpeed / 1024).toFixed(2) + "Mb\/s";
                      } else {
                        formatSpeed = uploadSpeed.toFixed(2) + "Kb\/s";
                      }
                      percentComplete = Math.round(evt.loaded * 100 / evt.total);
                      return console.log(percentComplete, ",", formatSpeed);
                    }
                  }, false);
                  xhr.onreadystatechange = function(response) {
                    var blkRet;
                    if (xhr.readyState === 4 && xhr.status === 200 && xhr.responseText !== "") {
                      blkRet = JSON.parse(xhr.responseText);
                      return $.post("/api/image/add", {
                        "aim_id": _room_id,
                        "md5": md5
                      }, function() {
                        content = "HQWEBIMG//" + md5;
                        $.ajax({
                          url: '/api/comment/submit',
                          type: 'POST',
                          dataType: 'json',
                          data: {
                            "app": WX_APP,
                            "aim_id": _room_id,
                            "content": content
                          },
                          success: function(data) {
                            return console.log(data);
                          },
                          error: function(error) {
                            return console.log(error);
                          }
                        });
                        return uploadNextFile();
                      });
                    }
                  };
                  startDate = new Date().getTime();
                  return xhr.send(formData);
                };
                return Qiniu_upload(file, upload_token, _room_id + "_" + md5);
              });
            };
            return worker.postMessage(bin);
          };
          return reader.readAsDataURL(file);
        };
        return uploadFile(blob);
      };
      return reader.readAsDataURL(blob);
    }
  }
});


