import React,{Component} from 'react'
import { Menu, Icon } from 'antd';
export default class NavItem extends Component {
    constructor(props, context) {
        super(props);
    }
    render() {
        return (<Menu.Item key={this.props.key}><Icon type={this.props.icon}/>{this.props.title}</Menu.Item>)
    }
}