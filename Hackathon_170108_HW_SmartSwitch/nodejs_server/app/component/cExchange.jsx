import React,{Component,Proptypes} from  'react'
import {bindActionCreators,combineReducers } from  'redux'
import {connect} from "react-redux"
import {addGift,addticket,subGift,changeName} from "../action/actions"
import ExchangeTip  from "./cExchangeTips.jsx"

class Exchange extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        return (
            <ExchangeTip  {...this.props}/>
        );
    }
}
function mapStateToProps(state) {
    return {
        coins: state.wealth.coins,
        tickets: state.wealth.tickets,
        gifts: state.wealth.gifts,
        username: state.info.username
    }
}
function mapDispatchToProps(dispatch) {
    return {
        todoWea: bindActionCreators({
            addGift, addticket, subGift
        }, dispatch),
        todoInfo: bindActionCreators({
            changeName
        }, dispatch)
    }
}

export default connect(mapStateToProps, mapDispatchToProps)(Exchange);